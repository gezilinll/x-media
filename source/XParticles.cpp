//
// Created by 林炳河 on 2019-09-03.
//

#include "XParticles.hpp"
#include "bgfx_utils.h"

NS_X_IMAGE_BEGIN

XParticles::XParticles() {
    mParticleSystem = new XParticleSystem();
}

XParticles::~XParticles() {
    mParticleSystem->shutdown();
}

void XParticles::init(uint16_t maxEmitters, bx::AllocatorI* allocator) {
    mParticleSystem->init(maxEmitters, allocator);
}

EmitterHandle XParticles::createEmitter(EmitterShape::Enum shape, EmitterDirection::Enum direction,
                                     uint32_t maxParticles) {
    return mParticleSystem->createEmitter(shape, direction, maxParticles);
}

EmitterSpriteHandle XParticles::createSprite(std::string filePath, bgfx::TextureFormat::Enum dstFormat) {
    bimg::ImageContainer *image = imageLoad(filePath.data(), dstFormat);
    return mParticleSystem->createSprite(image->m_width, image->m_height, image->m_data);
}

void XParticles::destroyEmitter(EmitterHandle handle) {
    mParticleSystem->destroyEmitter(handle);
}

void XParticles::updateEmitter(EmitterHandle handle, EmitterUniforms *uniforms) {
    mParticleSystem->updateEmitter(handle, uniforms);
}

void XParticles::updateViewAndEye(float *mtxView, const bx::Vec3 &eye) {
    mViewMatrix = mtxView;
    mEyePos = eye;
}

void XParticles::renderAtProgress(float progress, int index) {
    mParticleSystem->update(progress);
    mParticleSystem->render(index, mViewMatrix, mEyePos);
}

void XParticles::getAabb(EmitterHandle handle, Aabb &outAabb) {
    mParticleSystem->getAabb(handle, outAabb);
}
NS_X_IMAGE_END
