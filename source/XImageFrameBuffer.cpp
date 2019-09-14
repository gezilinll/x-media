//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFrameBuffer.hpp"
#include "XImageUtils.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
XImageFrameBuffer::XImageFrameBuffer() {
    mHandle = BGFX_INVALID_HANDLE;
}

XImageFrameBuffer::~XImageFrameBuffer() {
    XImageUtils::destroy(mHandle);
}

void XImageFrameBuffer::loadFromImage(std::string path) {
    if (bgfx::isValid(mHandle)) {
        LOGW("XImageFrameBuffer::loadFromImage recycle existed FrameBufferHandle, path=%s", path.data());
        XImageUtils::destroy(mHandle);
    }
    mImagePath = path;
    bgfx::TextureHandle textureHandle = loadTexture(path.data());
    if (!bgfx::isValid(textureHandle)) {
        LOGE("XImageFrameBuffer::loadFromImage failed, path=%s", path.data());
        return;
    }
    mHandle = bgfx::createFrameBuffer(1, &textureHandle, true);
}

void XImageFrameBuffer::create(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum format,
                               uint64_t textureFlags) {
    mHandle = bgfx::createFrameBuffer(width, height, format, textureFlags);
    if (!bgfx::isValid(mHandle)) {
        LOGE("XImageFrameBuffer::create failed, width=%d, height=%d", width, height);
    }
}

bimg::ImageContainer *XImageFrameBuffer::getImageContainer(bgfx::TextureFormat::Enum dstFormat) {
    if (mImagePath.empty()) {
        LOGE("XImageFrameBuffer::getImageContainer image path is invalid.");
        return nullptr;
    }
    return imageLoad(mImagePath.data(), dstFormat);
}

bgfx::TextureHandle XImageFrameBuffer::getTexture() {
    return bgfx::getTexture(mHandle);
}

bgfx::FrameBufferHandle XImageFrameBuffer::get() {
    return mHandle;
}
NS_X_IMAGE_END
