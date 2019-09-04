/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include "XParticleSystem.hpp"
#include "XImageUtils.hpp"
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

static int32_t particleSortFn(const void* _lhs, const void* _rhs)
{
    const ParticleSort& lhs = *(const ParticleSort*)_lhs;
    const ParticleSort& rhs = *(const ParticleSort*)_rhs;
    return lhs.dist > rhs.dist ? -1 : 1;
}

XParticleSystem::XParticleSystem() {
}

void XParticleSystem::init(uint16_t _maxEmitters, bx::AllocatorI *_allocator) {
    m_allocator = _allocator;

    if (NULL == _allocator)
    {
        static bx::DefaultAllocator allocator;
        m_allocator = &allocator;
    }

    m_emitterAlloc = bx::createHandleAlloc(m_allocator, _maxEmitters);
    m_emitter = (Emitter*)BX_ALLOC(m_allocator, sizeof(Emitter)*_maxEmitters);

    PosColorTexCoord0Vertex::init();

    m_num = 0;

    s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    m_texture  = bgfx::createTexture2D(
            SPRITE_TEXTURE_SIZE
            , SPRITE_TEXTURE_SIZE
            , false
            , 1
            , bgfx::TextureFormat::BGRA8
    );

    bgfx::RendererType::Enum type = bgfx::getRendererType();
    m_particleProgram = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_particle")
            , bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_particle")
            , true
    );
}

void XParticleSystem::shutdown() {
    bgfx::destroy(m_particleProgram);
    bgfx::destroy(m_texture);
    bgfx::destroy(s_texColor);

    bx::destroyHandleAlloc(m_allocator, m_emitterAlloc);
    BX_FREE(m_allocator, m_emitter);

    m_allocator = NULL;
}

EmitterSpriteHandle XParticleSystem::createSprite(uint16_t _width, uint16_t _height, const void *_data) {
    EmitterSpriteHandle handle = m_sprite.create(_width, _height);

    if (isValid(handle) )
    {
        const Pack2D& pack = m_sprite.get(handle);
        bgfx::updateTexture2D(
                m_texture
                , 0
                , 0
                , pack.m_x
                , pack.m_y
                , pack.m_width
                , pack.m_height
                , bgfx::copy(_data, pack.m_width*pack.m_height*4)
        );
    }

    return handle;
}

EmitterHandle XParticleSystem::createEmitter(XImage::EmitterShape::Enum _shape,
                                            XImage::EmitterDirection::Enum _direction, uint32_t _maxParticles) {
    EmitterHandle handle = { m_emitterAlloc->alloc() };

    if (UINT16_MAX != handle.idx)
    {
        m_emitter[handle.idx].create(_shape, _direction, _maxParticles, m_allocator);
    }

    return handle;
}

void XParticleSystem::updateEmitter(XImage::EmitterHandle _handle, const XImage::EmitterUniforms *_uniforms) {
    BX_CHECK(m_emitterAlloc.isValid(_handle.idx)
    , "destroyEmitter handle %d is not valid."
    , _handle.idx
    );

    Emitter& emitter = m_emitter[_handle.idx];

    if (NULL == _uniforms)
    {
        emitter.reset();
    }
    else
    {
        bx::memCopy(&emitter.m_uniforms, _uniforms, sizeof(EmitterUniforms) );
    }
}

void XParticleSystem::getAabb(XImage::EmitterHandle _handle, Aabb &_outAabb) {
    BX_CHECK(m_emitterAlloc.isValid(_handle.idx)
    , "getAabb handle %d is not valid."
    , _handle.idx
    );
    _outAabb = m_emitter[_handle.idx].m_aabb;
}

void XParticleSystem::destroyEmitter(XImage::EmitterHandle _handle) {
    BX_CHECK(m_emitterAlloc.isValid(_handle.idx)
    , "destroyEmitter handle %d is not valid."
    , _handle.idx
    );

    m_emitter[_handle.idx].destroy(m_allocator);
    m_emitterAlloc->free(_handle.idx);
}

void XParticleSystem::update(float _dt) {
    uint32_t numParticles = 0;
    for (uint16_t ii = 0, num = m_emitterAlloc->getNumHandles(); ii < num; ++ii)
    {
        const uint16_t idx = m_emitterAlloc->getHandleAt(ii);
        Emitter& emitter = m_emitter[idx];
        emitter.update(_dt);
        numParticles += emitter.m_num;
    }

    m_num = numParticles;
}

void XParticleSystem::render(uint8_t _view, const float *_mtxView, const bx::Vec3 &_eye) {
    if (0 != m_num)
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        const uint32_t numVertices = bgfx::getAvailTransientVertexBuffer(m_num*4, PosColorTexCoord0Vertex::ms_decl);
        const uint32_t numIndices  = bgfx::getAvailTransientIndexBuffer(m_num*6);
        const uint32_t max = bx::uint32_min(numVertices/4, numIndices/6);
        BX_WARN(m_num == max
        , "Truncating transient buffer for particles to maximum available (requested %d, available %d)."
        , m_num
        , max
        );

        if (0 < max)
        {
            bgfx::allocTransientBuffers(&tvb
                    , PosColorTexCoord0Vertex::ms_decl
                    , max*4
                    , &tib
                    , max*6
            );
            PosColorTexCoord0Vertex* vertices = (PosColorTexCoord0Vertex*)tvb.data;

            ParticleSort* particleSort = (ParticleSort*)BX_ALLOC(m_allocator, max*sizeof(ParticleSort) );

            uint32_t pos = 0;
            for (uint16_t ii = 0, numEmitters = m_emitterAlloc->getNumHandles(); ii < numEmitters; ++ii)
            {
                const uint16_t idx = m_emitterAlloc->getHandleAt(ii);
                Emitter& emitter = m_emitter[idx];

                const Pack2D& pack = m_sprite.get(emitter.m_uniforms.sprite);
                const float invTextureSize = 1.0f/SPRITE_TEXTURE_SIZE;
                const float uv[4] =
                        {
                                pack.m_x                  * invTextureSize,
                                pack.m_y                  * invTextureSize,
                                (pack.m_x + pack.m_width ) * invTextureSize,
                                (pack.m_y + pack.m_height) * invTextureSize,
                        };

                pos += emitter.render(uv, _mtxView, _eye, pos, max, particleSort, vertices);
            }

            qsort(particleSort
                    , max
                    , sizeof(ParticleSort)
                    , particleSortFn
            );

            uint16_t* indices = (uint16_t*)tib.data;
            for (uint32_t ii = 0; ii < max; ++ii)
            {
                const ParticleSort& sort = particleSort[ii];
                uint16_t* index = &indices[ii*6];
                uint16_t idx = (uint16_t)sort.idx;
                index[0] = idx*4+0;
                index[1] = idx*4+1;
                index[2] = idx*4+2;
                index[3] = idx*4+2;
                index[4] = idx*4+3;
                index[5] = idx*4+0;
            }

            BX_FREE(m_allocator, particleSort);

            bgfx::setState(0
                           | BGFX_STATE_WRITE_RGB
                           | BGFX_STATE_WRITE_A
                           | BGFX_STATE_DEPTH_TEST_LESS
                           | BGFX_STATE_CULL_CW
                           | BGFX_STATE_BLEND_NORMAL
            );
            bgfx::setVertexBuffer(0, &tvb);
            bgfx::setIndexBuffer(&tib);
            bgfx::setTexture(0, s_texColor, m_texture);
            bgfx::submit(_view, m_particleProgram);
        }
    }
}

NS_X_IMAGE_END