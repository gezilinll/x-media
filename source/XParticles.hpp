//
// Created by 林炳河 on 2019-09-03.
//

#ifndef GPUIMAGE_X_XPARTICLES_HPP
#define GPUIMAGE_X_XPARTICLES_HPP

#include "XMacros.hpp"
#include "XParticleSystem.hpp"
#include "XImageInput.hpp"
#include "XImageOutput.hpp"

NS_X_IMAGE_BEGIN
class XParticles: public XImageInput, public XImageOutput {
public:
    XParticles();

    ~XParticles();

    void init(uint16_t maxEmitters = 64, bx::AllocatorI* allocator = NULL);

    EmitterHandle createEmitter(EmitterShape::Enum shape, EmitterDirection::Enum direction, uint32_t maxParticles);

    EmitterSpriteHandle createSprite(std::string filePath, bgfx::TextureFormat::Enum dstFormat);

    void updateEmitter(EmitterHandle handle, EmitterUniforms *uniforms = nullptr);

    void updateViewAndEye(float *mtxView, const bx::Vec3 &eye);

    void renderAtProgress(float progress, int index) override;

    void destroyEmitter(EmitterHandle handle);

    void getAabb(EmitterHandle handle, Aabb& outAabb);

private:
    XParticleSystem *mParticleSystem;
    float *mViewMatrix;
    bx::Vec3 mEyePos;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XPARTICLES_HPP
