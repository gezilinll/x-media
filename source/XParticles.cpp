#include "XParticles.hpp"
#include "XImage.hpp"
#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include "ps/vs_particle.bin.h"
#include "ps/fs_particle.bin.h"

NS_X_IMAGE_BEGIN
bgfx::VertexDecl PosColorTexCoord0Vertex::ms_decl;

static const bgfx::EmbeddedShader s_embeddedShaders[] =
        {
                BGFX_EMBEDDED_SHADER(vs_particle),
                BGFX_EMBEDDED_SHADER(fs_particle),

                BGFX_EMBEDDED_SHADER_END()
        };

static int32_t particleSortFn(const void *_lhs, const void *_rhs) {
    const ParticleSort &lhs = *(const ParticleSort *) _lhs;
    const ParticleSort &rhs = *(const ParticleSort *) _rhs;
    return lhs.dist > rhs.dist ? -1 : 1;
}


XParticles::XParticles() {
    mViewMatrix = nullptr;
    mAllocator = nullptr;
    mEmitterAlloc = nullptr;
    mEmitter = nullptr;

    mTexColor = {bgfx::kInvalidHandle};
    mTexture = {bgfx::kInvalidHandle};
    mParticleProgram = {bgfx::kInvalidHandle};
}

XParticles::~XParticles() {
    bgfx::destroy(mParticleProgram);
    bgfx::destroy(mTexture);
    bgfx::destroy(mTexColor);

    bx::destroyHandleAlloc(mAllocator, mEmitterAlloc);
    BX_FREE(mAllocator, mEmitter);

    mAllocator = NULL;
}

void XParticles::init(uint16_t maxEmitters, bx::AllocatorI *allocator) {
    mAllocator = allocator;

    if (nullptr == allocator) {
        static bx::DefaultAllocator allocator;
        mAllocator = &allocator;
    }

    mEmitterAlloc = bx::createHandleAlloc(mAllocator, maxEmitters);
    mEmitter = (XEmitter *) BX_ALLOC(mAllocator, sizeof(XEmitter) * maxEmitters);

    PosColorTexCoord0Vertex::init();

    mNum = 0;

    mTexColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    mTexture = bgfx::createTexture2D(
            SPRITE_TEXTURE_SIZE, SPRITE_TEXTURE_SIZE, false, 1, bgfx::TextureFormat::BGRA8);

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    mParticleProgram = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_particle"),
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_particle"), true);
}

EmitterHandle XParticles::createEmitter(EmitterShape::Enum shape, EmitterDirection::Enum direction,
                                        uint32_t maxParticles) {
    EmitterHandle handle = {mEmitterAlloc->alloc()};

    if (UINT16_MAX != handle.idx) {
        mEmitter[handle.idx].create(shape, direction, maxParticles, mAllocator);
    }

    return handle;
}

EmitterSpriteHandle XParticles::createSprite(std::string filePath, bgfx::TextureFormat::Enum dstFormat) {
    bimg::ImageContainer *image = imageLoad(filePath.data(), dstFormat);
    EmitterSpriteHandle handle = mSprite.create(image->m_width, image->m_height);

    if (isValid(handle)) {
        const Pack2D &pack = mSprite.get(handle);
        bgfx::updateTexture2D(
                mTexture, 0, 0, pack.m_x, pack.m_y, pack.m_width, pack.m_height,
                bgfx::copy(image->m_data, pack.m_width * pack.m_height * 4));
    }
    bimg::imageFree(image);
    return handle;

}

void XParticles::destroyEmitter(EmitterHandle handle) {
    BX_CHECK(mEmitterAlloc.isValid(handle.idx), "destroyEmitter handle %d is not valid.", handle.idx);

    mEmitter[handle.idx].destroy(mAllocator);
    mEmitterAlloc->free(handle.idx);
}

void XParticles::updateEmitter(EmitterHandle handle, EmitterUniforms *uniforms) {
    BX_CHECK(mEmitterAlloc.isValid(handle.idx), "destroyEmitter handle %d is not valid.", handle.idx);

    XEmitter &emitter = mEmitter[handle.idx];

    if (nullptr == uniforms) {
        emitter.reset();
    } else {
        bx::memCopy(&emitter.m_uniforms, uniforms, sizeof(EmitterUniforms));
    }
}

void XParticles::updateViewAndEye(float *mtxView, const bx::Vec3 &eye) {
    mViewMatrix = mtxView;
    mEyePos = eye;
}

void XParticles::submit(int index) {
    uint32_t numParticles = 0;
    for (uint16_t ii = 0, num = mEmitterAlloc->getNumHandles(); ii < num; ++ii) {
        const uint16_t idx = mEmitterAlloc->getHandleAt(ii);
        XEmitter &emitter = mEmitter[idx];
        emitter.update(0.5);
        numParticles += emitter.m_num;
    }

    mNum = numParticles;

    if (0 != mNum) {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        const uint32_t numVertices = bgfx::getAvailTransientVertexBuffer(mNum * 4,
                                                                         PosColorTexCoord0Vertex::ms_decl);
        const uint32_t numIndices = bgfx::getAvailTransientIndexBuffer(mNum * 6);
        const uint32_t max = bx::uint32_min(numVertices / 4, numIndices / 6);
        BX_WARN(mNum == max,
                "Truncating transient buffer for particles to maximum available (requested %d, available %d).",
                mNum, max);

        if (0 < max) {
            bgfx::allocTransientBuffers(&tvb, PosColorTexCoord0Vertex::ms_decl, max * 4, &tib, max * 6);
            PosColorTexCoord0Vertex *vertices = (PosColorTexCoord0Vertex *) tvb.data;

            ParticleSort *particleSort = (ParticleSort *) BX_ALLOC(mAllocator, max * sizeof(ParticleSort));

            uint32_t pos = 0;
            for (uint16_t ii = 0, numEmitters = mEmitterAlloc->getNumHandles(); ii < numEmitters; ++ii) {
                const uint16_t idx = mEmitterAlloc->getHandleAt(ii);
                XEmitter &emitter = mEmitter[idx];

                const Pack2D &pack = mSprite.get(emitter.m_uniforms.sprite);
                const float invTextureSize = 1.0f / SPRITE_TEXTURE_SIZE;
                const float uv[4] =
                        {
                                pack.m_x * invTextureSize,
                                pack.m_y * invTextureSize,
                                (pack.m_x + pack.m_width) * invTextureSize,
                                (pack.m_y + pack.m_height) * invTextureSize,
                        };

                pos += emitter.render(uv, mViewMatrix, mEyePos, pos, max, particleSort, vertices);
            }

            qsort(particleSort, max, sizeof(ParticleSort), particleSortFn);

            uint16_t *indices = (uint16_t *) tib.data;
            for (uint32_t ii = 0; ii < max; ++ii) {
                const ParticleSort &sort = particleSort[ii];
                uint16_t *index = &indices[ii * 6];
                uint16_t idx = (uint16_t) sort.idx;
                index[0] = idx * 4 + 0;
                index[1] = idx * 4 + 1;
                index[2] = idx * 4 + 2;
                index[3] = idx * 4 + 2;
                index[4] = idx * 4 + 3;
                index[5] = idx * 4 + 0;
            }

            BX_FREE(mAllocator, particleSort);

            bgfx::touch(index);
            bgfx::setState(0
                           | BGFX_STATE_WRITE_RGB
                           | BGFX_STATE_WRITE_A
                           | BGFX_STATE_DEPTH_TEST_LESS
                           | BGFX_STATE_CULL_CW
                           | BGFX_STATE_BLEND_NORMAL);
            bgfx::setVertexBuffer(0, &tvb);
            bgfx::setIndexBuffer(&tib);
            bgfx::setTexture(0, mTexColor, mTexture);
            bgfx::submit(index, mParticleProgram);
        }
    }
}

void XParticles::getAabb(EmitterHandle handle, Aabb &outAabb) {
    BX_CHECK(mEmitterAlloc.isValid(handle.idx), "getAabb handle %d is not valid.", handle.idx);
    outAabb = mEmitter[handle.idx].m_aabb;
}
NS_X_IMAGE_END
