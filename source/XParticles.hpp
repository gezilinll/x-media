//
// Created by 林炳河 on 2019-09-03.
//

#ifndef GPUIMAGE_X_XPARTICLES_HPP
#define GPUIMAGE_X_XPARTICLES_HPP

#include "XMacros.hpp"
#include "XImageInput.hpp"
#include "XImageOutput.hpp"
#include "packrect.h"
#include "bounds.h"
#include "XEmitter.h"
#include <bx/allocator.h>
#include <bx/easing.h>
#include <bx/rng.h>
#include <bx/handlealloc.h>

NS_X_IMAGE_BEGIN

#define SPRITE_TEXTURE_SIZE 1024
template<uint16_t MaxHandlesT = 256, uint16_t TextureSizeT = 1024>
struct SpriteT
{
    SpriteT() : rectPack2Dt(TextureSizeT, TextureSizeT) {}

    EmitterSpriteHandle create(uint16_t width, uint16_t height) {
        EmitterSpriteHandle handle = { bx::kInvalidHandle };

        if (handleAllocT.getNumHandles() < handleAllocT.getMaxHandles() )
        {
            Pack2D pack;
            if (rectPack2Dt.find(width, height, pack) )
            {
                handle.idx = handleAllocT.alloc();
                pack2D[handle.idx] = pack;
            }
        }

        return handle;
    }

    void destroy(EmitterSpriteHandle _sprite) {
        const Pack2D& pack = pack2D[_sprite.idx];
        rectPack2Dt.clear(pack);
        handleAllocT.free(_sprite.idx);
    }

    const Pack2D& get(EmitterSpriteHandle _sprite) const {
        return pack2D[_sprite.idx];
    }

    bx::HandleAllocT<MaxHandlesT> handleAllocT;
    Pack2D                        pack2D[MaxHandlesT];
    RectPack2DT<256>              rectPack2Dt;
};

template<typename Ty>
inline bool isValid(Ty handle)
{
    return handle.idx != UINT16_MAX;
}

class XParticles: public XImageInput, public XImageOutput {
public:
    XParticles();

    ~XParticles();

    /**
     * @brief init particle data, must call first
     * @param maxEmitters max number of emitters
     * @param allocator memory allocator
     */
    void init(uint16_t maxEmitters = 64, bx::AllocatorI* allocator = NULL);

    /**
     * @brief create one emitter
     * @param shape shape of emission
     * @param direction direction of emission
     * @param maxParticles max number of particle
     * @return created emitter handle
     */
    EmitterHandle createEmitter(EmitterShape::Enum shape, EmitterDirection::Enum direction, uint32_t maxParticles);

    /**
     * @brief create one sprite
     * @param filePath file path of sprite
     * @param dstFormat the format sprite need to loaded
     * @return created sprite handle
     */
    EmitterSpriteHandle createSprite(std::string filePath, bgfx::TextureFormat::Enum dstFormat);

    /**
     * @brief update selected handle
     * @param handle selected handle
     * @param uniforms updated parameter
     */
    void updateEmitter(EmitterHandle handle, EmitterUniforms *uniforms = nullptr);

    /**
     * @brief update view matrix and eye position
     * @param mtxView view matrix
     * @param eye eye position
     */
    void updateViewAndEye(float *mtxView, const bx::Vec3 &eye);

    void renderAtProgress(float progress, int index) override;

    /**
     * @brief destroy selected emitter
     * @param handle selected emitter
     */
    void destroyEmitter(EmitterHandle handle);

    /**
     * @brief get aabb of emission
     * @param handle selected handle
     * @param outAabb aabb value
     */
    void getAabb(EmitterHandle handle, Aabb& outAabb);

private:
    float *mViewMatrix;
    bx::Vec3 mEyePos;

    bx::AllocatorI* mAllocator;

    bx::HandleAlloc* mEmitterAlloc;
    XEmitter* mEmitter;

    typedef SpriteT<256, SPRITE_TEXTURE_SIZE> Sprite;
    Sprite mSprite;

    bgfx::UniformHandle mTexColor;
    bgfx::TextureHandle mTexture;
    bgfx::ProgramHandle mParticleProgram;

    uint32_t mNum;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XPARTICLES_HPP
