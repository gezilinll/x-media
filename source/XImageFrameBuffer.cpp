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

void XImageFrameBuffer::loadFromPicture(std::string path) {
    if (bgfx::isValid(mHandle)) {
        LOGW("XImageFrameBuffer::loadFromPicture recycle existed FrameBufferHandle, path=%s", path.data());
        XImageUtils::destroy(mHandle);
    }
    bgfx::TextureHandle textureHandle = loadTexture(path.data());
    if (!bgfx::isValid(textureHandle)) {
        LOGE("XImageFrameBuffer::loadFromPicture failed, path=%s", path.data());
        return;
    }
    mHandle = bgfx::createFrameBuffer(1, &textureHandle, true);
}


bgfx::TextureHandle XImageFrameBuffer::getTexture() {
    return bgfx::getTexture(mHandle);
}
NS_X_IMAGE_END
