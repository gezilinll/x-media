//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFrameBuffer.hpp"
#include "XImageUtils.hpp"

XImageFrameBuffer::XImageFrameBuffer() {
    mHandle = BGFX_INVALID_HANDLE;
}

void XImageFrameBuffer::loadFromPicture(std::string path) {
    bgfx::TextureHandle textureHandle = XImageUtils::loadTexture(path.data());
    if (!bgfx::isValid(textureHandle)) {
        //todo: log
    }
    mHandle = bgfx::createFrameBuffer(1, &textureHandle, true);
}


bgfx::TextureHandle XImageFrameBuffer::getTexture() {
    return bgfx::getTexture(mHandle);
}