//
// Created by 林炳河 on 2019-07-29.
//

#ifndef GPUIMAGE_X_EMITTER_H
#define GPUIMAGE_X_EMITTER_H

#include "XMacros.hpp"
#include "bounds.h"
#include <bx/easing.h>
#include <bx/rng.h>
#include <bgfx/bgfx.h>

NS_X_IMAGE_BEGIN

struct EmitterHandle       { uint16_t idx; };
struct EmitterSpriteHandle { uint16_t idx; };

struct EmitterShape
{
    enum Enum
    {
        Sphere,
        Hemisphere,
        Circle,
        Disc,
        Rect,

        Count
    };
};

struct EmitterDirection
{
    enum Enum
    {
        Up,
        Outward,

        Count
    };
};

struct EmitterUniforms
{
    void reset();

    float m_position[3];
    float m_angle[3];

    float m_blendStart[2];
    float m_blendEnd[2];
    float m_offsetStart[2];
    float m_offsetEnd[2];
    float m_scaleStart[2];
    float m_scaleEnd[2];
    float m_lifeSpan[2];
    float m_gravityScale;

    uint32_t m_rgba[5];
    uint32_t m_particlesPerSecond;

    bx::Easing::Enum m_easePos;
    bx::Easing::Enum m_easeRgba;
    bx::Easing::Enum m_easeBlend;
    bx::Easing::Enum m_easeScale;

    EmitterSpriteHandle m_handle;
};

struct Particle
{
    bx::Vec3 start;
    bx::Vec3 end[2];
    float blendStart;
    float blendEnd;
    float scaleStart;
    float scaleEnd;

    uint32_t rgba[5];

    float life;
    float lifeSpan;
};

struct ParticleSort
{
    float    dist;
    uint32_t idx;
};

struct PosColorTexCoord0Vertex
{
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;
    float m_u;
    float m_v;
    float m_blend;
    float m_angle;

    static void init()
    {
        ms_decl
                .begin()
                .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
                .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float)
                .end();
    }

    static bgfx::VertexDecl ms_decl;
};

class Emitter {
public:
    void create(EmitterShape::Enum _shape, EmitterDirection::Enum _direction, uint32_t _maxParticles, bx::AllocatorI *allocatorI);

    void destroy(bx::AllocatorI *allocatorI);

    void reset();

    void update(float _dt);

    void spawn(float _dt);

    uint32_t render(const float _uv[4], const float* _mtxView, const bx::Vec3& _eye, uint32_t _first, uint32_t _max, ParticleSort* _outSort, PosColorTexCoord0Vertex* _outVertices);

public:
    EmitterShape::Enum     m_shape;
    EmitterDirection::Enum m_direction;

    float           m_dt;
    bx::RngMwc      m_rng;
    EmitterUniforms m_uniforms;

    Aabb m_aabb;

    Particle* m_particles;
    uint32_t m_num;
    uint32_t m_max;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_EMITTER_H
