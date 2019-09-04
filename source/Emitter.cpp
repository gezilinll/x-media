//
// Created by 林炳河 on 2019-07-29.
//

#include "Emitter.h"
#include <bx/allocator.h>

NS_X_IMAGE_BEGIN
inline uint32_t toAbgr(float _rr, float _gg, float _bb, float _aa)
{
    return 0
           | (uint8_t(_rr*255.0f)<< 0)
           | (uint8_t(_gg*255.0f)<< 8)
           | (uint8_t(_bb*255.0f)<<16)
           | (uint8_t(_aa*255.0f)<<24)
            ;
}

void EmitterUniforms::reset() {
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    angle[0] = 0.0f;
    angle[1] = 0.0f;
    angle[2] = 0.0f;

    particlesPerSecond = 0;

    offsetStart[0] = 0.0f;
    offsetStart[1] = 1.0f;
    offsetEnd[0]   = 2.0f;
    offsetEnd[1]   = 3.0f;

    rgba[0] = 0x00ffffff;
    rgba[1] = UINT32_MAX;
    rgba[2] = UINT32_MAX;
    rgba[3] = UINT32_MAX;
    rgba[4] = 0x00ffffff;

    blendStart[0] = 0.8f;
    blendStart[1] = 1.0f;
    blendEnd[0]   = 0.0f;
    blendEnd[1]   = 0.2f;

    scaleStart[0] = 0.1f;
    scaleStart[1] = 0.2f;
    scaleEnd[0]   = 0.3f;
    scaleEnd[1]   = 0.4f;

    lifeSpan[0]   = 1.0f;
    lifeSpan[1]   = 2.0f;

    gravityScale  = 0.0f;

    easePos   = bx::Easing::Linear;
    easeRgba  = bx::Easing::Linear;
    easeBlend = bx::Easing::Linear;
    easeScale = bx::Easing::Linear;
}

void Emitter::reset() {
    m_dt = 0.0f;
    m_uniforms.reset();
    m_num = 0;
    bx::memSet(&m_aabb, 0, sizeof(Aabb) );

    m_rng.reset();
}

void Emitter::update(float _dt) {
    uint32_t num = m_num;
    for (uint32_t ii = 0; ii < num; ++ii)
    {
        Particle& particle = m_particles[ii];
        particle.life += _dt * 1.0f/particle.lifeSpan;

        if (particle.life > 1.0f)
        {
            if (ii != num-1)
            {
                bx::memCopy(&particle, &m_particles[num-1], sizeof(Particle) );
                --ii;
            }

            --num;
        }
    }

    m_num = num;

    if (0 < m_uniforms.particlesPerSecond)
    {
        spawn(_dt);
    }
}

void Emitter::spawn(float _dt) {
    float mtx[16];
    bx::mtxSRT(mtx
            , 1.0f, 1.0f, 1.0f
            , m_uniforms.angle[0],    m_uniforms.angle[1],    m_uniforms.angle[2]
            , m_uniforms.position[0], m_uniforms.position[1], m_uniforms.position[2]
    );

    const float timePerParticle = 1.0f/m_uniforms.particlesPerSecond;
    m_dt += _dt;
    const uint32_t numParticles = uint32_t(m_dt / timePerParticle);
    m_dt -= numParticles * timePerParticle;

    constexpr bx::Vec3 up = { 0.0f, 1.0f, 0.0f };

    float time = 0.0f;
    for (uint32_t ii = 0
            ; ii < numParticles && m_num < m_max
            ; ++ii
            )
    {
        Particle& particle = m_particles[m_num];
        m_num++;

        bx::Vec3 pos;
        switch (m_shape)
        {
            default:
            case EmitterShape::Sphere:
                pos = bx::randUnitSphere(&m_rng);
                break;

            case EmitterShape::Hemisphere:
                pos = bx::randUnitHemisphere(&m_rng, up);
                break;

            case EmitterShape::Circle:
                pos = bx::randUnitCircle(&m_rng);
                break;

            case EmitterShape::Disc:
            {
                const bx::Vec3 tmp = bx::randUnitCircle(&m_rng);
                pos = bx::mul(tmp, bx::frnd(&m_rng) );
            }
                break;

            case EmitterShape::Rect:
                pos =
                        {
                                bx::frndh(&m_rng),
                                0.0f,
                                bx::frndh(&m_rng),
                        };
                break;
        }

        bx::Vec3 dir;
        switch (m_direction)
        {
            default:
            case EmitterDirection::Up:
                dir = up;
                break;

            case EmitterDirection::Outward:
                dir = bx::normalize(pos);
                break;
        }

        const float startOffset = bx::lerp(m_uniforms.offsetStart[0], m_uniforms.offsetStart[1], bx::frnd(&m_rng) );
        const bx::Vec3 start = bx::mul(pos, startOffset);

        const float endOffset = bx::lerp(m_uniforms.offsetEnd[0], m_uniforms.offsetEnd[1], bx::frnd(&m_rng) );
        const bx::Vec3 tmp1 = bx::mul(dir, endOffset);
        const bx::Vec3 end  = bx::add(tmp1, start);

        particle.life = time;
        particle.lifeSpan = bx::lerp(m_uniforms.lifeSpan[0], m_uniforms.lifeSpan[1], bx::frnd(&m_rng) );

        const bx::Vec3 gravity = { 0.0f, -9.81f * m_uniforms.gravityScale * bx::square(particle.lifeSpan), 0.0f };

        particle.start  = bx::mul(start, mtx);
        particle.end[0] = bx::mul(end,   mtx);
        particle.end[1] = bx::add(particle.end[0], gravity);

        bx::memCopy(particle.rgba, m_uniforms.rgba, BX_COUNTOF(m_uniforms.rgba)*sizeof(uint32_t) );

        particle.blendStart = bx::lerp(m_uniforms.blendStart[0], m_uniforms.blendStart[1], bx::frnd(&m_rng) );
        particle.blendEnd   = bx::lerp(m_uniforms.blendEnd[0],   m_uniforms.blendEnd[1],   bx::frnd(&m_rng) );

        particle.scaleStart = bx::lerp(m_uniforms.scaleStart[0], m_uniforms.scaleStart[1], bx::frnd(&m_rng) );
        particle.scaleEnd   = bx::lerp(m_uniforms.scaleEnd[0],   m_uniforms.scaleEnd[1],   bx::frnd(&m_rng) );

        time += timePerParticle;
    }
}

void Emitter::create(XImage::EmitterShape::Enum _shape, XImage::EmitterDirection::Enum _direction,
                     uint32_t _maxParticles, bx::AllocatorI *allocatorI) {
    reset();

    m_shape     = _shape;
    m_direction = _direction;
    m_max       = _maxParticles;
    m_particles = (Particle*)BX_ALLOC(allocatorI, m_max*sizeof(Particle) );
}

void Emitter::destroy(bx::AllocatorI *allocatorI) {
    BX_FREE(allocatorI, m_particles);
    m_particles = NULL;
}

uint32_t Emitter::render(const float *_uv, const float *_mtxView, const bx::Vec3 &_eye, uint32_t _first, uint32_t _max,
                         XImage::ParticleSort *_outSort, XImage::PosColorTexCoord0Vertex *_outVertices) {
    bx::EaseFn easeRgba  = bx::getEaseFunc(m_uniforms.easeRgba);
    bx::EaseFn easePos   = bx::getEaseFunc(m_uniforms.easePos);
    bx::EaseFn easeBlend = bx::getEaseFunc(m_uniforms.easeBlend);
    bx::EaseFn easeScale = bx::getEaseFunc(m_uniforms.easeScale);

    Aabb aabb =
            {
                    {  bx::kInfinity,  bx::kInfinity,  bx::kInfinity },
                    { -bx::kInfinity, -bx::kInfinity, -bx::kInfinity },
            };

    for (uint32_t jj = 0, num = m_num, current = _first
            ; jj < num && current < _max
            ; ++jj, ++current
            )
    {
        const Particle& particle = m_particles[jj];

        const float ttPos   = easePos(particle.life);
        const float ttScale = easeScale(particle.life);
        const float ttBlend = bx::clamp(easeBlend(particle.life), 0.0f, 1.0f);
        const float ttRgba  = bx::clamp(easeRgba(particle.life),  0.0f, 1.0f);

        const bx::Vec3 p0  = bx::lerp(particle.start,  particle.end[0], ttPos);
        const bx::Vec3 p1  = bx::lerp(particle.end[0], particle.end[1], ttPos);
        const bx::Vec3 pos = bx::lerp(p0, p1, ttPos);

        ParticleSort& sort = _outSort[current];
        const bx::Vec3 tmp0 = bx::sub(_eye, pos);
        sort.dist = bx::length(tmp0);
        sort.idx  = current;

        uint32_t idx = uint32_t(ttRgba*4);
        float ttmod = bx::mod(ttRgba, 0.25f)/0.25f;
        uint32_t rgbaStart = particle.rgba[idx];
        uint32_t rgbaEnd   = particle.rgba[idx+1];

        float rr = bx::lerp( ( (uint8_t*)&rgbaStart)[0], ( (uint8_t*)&rgbaEnd)[0], ttmod)/255.0f;
        float gg = bx::lerp( ( (uint8_t*)&rgbaStart)[1], ( (uint8_t*)&rgbaEnd)[1], ttmod)/255.0f;
        float bb = bx::lerp( ( (uint8_t*)&rgbaStart)[2], ( (uint8_t*)&rgbaEnd)[2], ttmod)/255.0f;
        float aa = bx::lerp( ( (uint8_t*)&rgbaStart)[3], ( (uint8_t*)&rgbaEnd)[3], ttmod)/255.0f;

        float blend = bx::lerp(particle.blendStart, particle.blendEnd, ttBlend);
        float scale = bx::lerp(particle.scaleStart, particle.scaleEnd, ttScale);

        uint32_t abgr = toAbgr(rr, gg, bb, aa);

        const bx::Vec3 udir = { _mtxView[0]*scale, _mtxView[4]*scale, _mtxView[8]*scale };
        const bx::Vec3 vdir = { _mtxView[1]*scale, _mtxView[5]*scale, _mtxView[9]*scale };

        PosColorTexCoord0Vertex* vertex = &_outVertices[current*4];

        const bx::Vec3 ul = bx::sub(bx::sub(pos, udir), vdir);
        bx::store(&vertex->m_x, ul);
        aabbExpand(aabb, ul);
        vertex->m_abgr  = abgr;
        vertex->m_u     = _uv[0];
        vertex->m_v     = _uv[1];
        vertex->m_blend = blend;
        ++vertex;

        const bx::Vec3 ur = bx::sub(bx::add(pos, udir), vdir);
        bx::store(&vertex->m_x, ur);
        aabbExpand(aabb, ur);
        vertex->m_abgr  = abgr;
        vertex->m_u     = _uv[2];
        vertex->m_v     = _uv[1];
        vertex->m_blend = blend;
        ++vertex;

        const bx::Vec3 br = bx::add(bx::add(pos, udir), vdir);
        bx::store(&vertex->m_x, br);
        aabbExpand(aabb, br);
        vertex->m_abgr  = abgr;
        vertex->m_u     = _uv[2];
        vertex->m_v     = _uv[3];
        vertex->m_blend = blend;
        ++vertex;

        const bx::Vec3 bl = bx::add(bx::sub(pos, udir), vdir);
        bx::store(&vertex->m_x, bl);
        aabbExpand(aabb, bl);
        vertex->m_abgr  = abgr;
        vertex->m_u     = _uv[0];
        vertex->m_v     = _uv[3];
        vertex->m_blend = blend;
        ++vertex;
    }

    m_aabb = aabb;

    return m_num;
}
NS_X_IMAGE_END