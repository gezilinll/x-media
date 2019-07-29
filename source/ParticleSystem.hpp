/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef PARTICLE_SYSTEM_H_HEADER_GUARD
#define PARTICLE_SYSTEM_H_HEADER_GUARD

#include <bx/allocator.h>
#include <bx/easing.h>
#include <bx/rng.h>
#include <bx/handlealloc.h>
#include "XBounds.hpp"
#include "Emitter.h"
#include "PackRect.hpp"

NS_X_IMAGE_BEGIN

#define SPRITE_TEXTURE_SIZE 1024
template<uint16_t MaxHandlesT = 256, uint16_t TextureSizeT = 1024>
struct SpriteT
{
    SpriteT() : m_ra(TextureSizeT, TextureSizeT) {}

    EmitterSpriteHandle create(uint16_t _width, uint16_t _height) {
        EmitterSpriteHandle handle = { bx::kInvalidHandle };

        if (m_handleAlloc.getNumHandles() < m_handleAlloc.getMaxHandles() )
        {
            Pack2D pack;
            if (m_ra.find(_width, _height, pack) )
            {
                handle.idx = m_handleAlloc.alloc();
                m_pack[handle.idx] = pack;
            }
        }

        return handle;
    }

    void destroy(EmitterSpriteHandle _sprite) {
        const Pack2D& pack = m_pack[_sprite.idx];
        m_ra.clear(pack);
        m_handleAlloc.free(_sprite.idx);
    }

    const Pack2D& get(EmitterSpriteHandle _sprite) const {
        return m_pack[_sprite.idx];
    }

    bx::HandleAllocT<MaxHandlesT> m_handleAlloc;
    Pack2D                        m_pack[MaxHandlesT];
    RectPack2DT<256>              m_ra;
};

template<typename Ty>
inline bool isValid(Ty _handle)
{
	return _handle.idx != UINT16_MAX;
}

class ParticleSystem {
public:
    void init(uint16_t _maxEmitters = 64, bx::AllocatorI* _allocator = NULL);

    void shutdown();

    EmitterSpriteHandle createSprite(uint16_t _width, uint16_t _height, const void* _data);

    void destroy(EmitterSpriteHandle _handle);

    EmitterHandle createEmitter(EmitterShape::Enum _shape, EmitterDirection::Enum _direction, uint32_t _maxParticles);

    void updateEmitter(EmitterHandle _handle, const EmitterUniforms* _uniforms = NULL);

    void getAabb(EmitterHandle _handle, Aabb& _outAabb);

    void destroyEmitter(EmitterHandle _handle);

    void update(float _dt);

    void render(uint8_t _view, const float* _mtxView, const bx::Vec3& _eye);

private:
    bx::AllocatorI* m_allocator;

    bx::HandleAlloc* m_emitterAlloc;
    Emitter* m_emitter;

    typedef SpriteT<256, SPRITE_TEXTURE_SIZE> Sprite;
    Sprite m_sprite;

    bgfx::UniformHandle s_texColor;
    bgfx::TextureHandle m_texture;
    bgfx::ProgramHandle m_particleProgram;

    uint32_t m_num;
};

NS_X_IMAGE_END

#endif // PARTICLE_SYSTEM_H_HEADER_GUARD
