//
// Created by 林炳河 on 2019/9/22.
//

#include "XFrameBufferPool.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
std::vector<XFrameBuffer *> XFrameBufferPool::sBuffers;
const int MAX_SIZE = 50;

XFrameBuffer* XFrameBufferPool::get(int width, int height) {
    XFrameBuffer *result = getFromPool(width, height);
    if (result == nullptr) {
        LOGE("[XFrameBufferPool::get|size] width=%d, height=%d", width, height);
        bgfx::FrameBufferHandle handle = bgfx::createFrameBuffer(width, height,
                                                                 bgfx::TextureFormat::BGRA8,
                                                                 BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);
        result = new XFrameBuffer(handle, width, height);
        sBuffers.push_back(result);
    }
    return result;
}

XFrameBuffer * XFrameBufferPool::get(std::string path, int width, int height) {
    if (width <= 0 || height <= 0) {
        bimg::ImageContainer *container = imageLoad(path.data(), bgfx::TextureFormat::BGRA8);
        width = container->m_width;
        height = container->m_height;
        bimg::imageFree(container);
    }

    XFrameBuffer *result = getFromPool(width, height);
    if (result == nullptr) {
        LOGE("[XFrameBufferPool::get] path=%s, width=%d, height=%d",
             path.data(), width, height);
        result = new XFrameBuffer(BGFX_INVALID_HANDLE, width, height);
        sBuffers.push_back(result);
    }
    bgfx::TextureHandle textureHandle = loadTexture(path.data());
    if (!bgfx::isValid(textureHandle)) {
        LOGE("[XFrameBufferPool::get|path] loadTexture failed, path=%s", path.data());
    }
    bgfx::FrameBufferHandle frameBufferHandle = bgfx::createFrameBuffer(1, &textureHandle, true);
    result->setFrameBufferHandle(frameBufferHandle);
    return result;
}

XFrameBuffer* XFrameBufferPool::getFromPool(int width, int height) {
    for (XFrameBuffer* buffer : sBuffers) {
        if (!buffer->isOccupied() && buffer->isSameSize(width, height)) {
            buffer->setOccupied(true);
            return buffer;
        }
    }
    return nullptr;
}

void XFrameBufferPool::recycle(XImageNS::XFrameBuffer *buffer) {
    if (buffer == nullptr) {
        return;
    }
    buffer->setOccupied(false);
}

void XFrameBufferPool::destroy() {
    for (XFrameBuffer* buffer : sBuffers) {
        SAFE_DELETE(buffer);
    }
    sBuffers.clear();
}
NS_X_IMAGE_END