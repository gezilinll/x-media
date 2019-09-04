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

bimg::ImageContainer *XImageFrameBuffer::getImageContainer(bgfx::TextureFormat::Enum dstFormat) {
    return imageLoad(mImagePath.data(), dstFormat);
}

bgfx::TextureHandle XImageFrameBuffer::getTexture() {
    return bgfx::getTexture(mHandle);
}
NS_X_IMAGE_END
