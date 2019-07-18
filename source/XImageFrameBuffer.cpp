//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFrameBuffer.hpp"

void XImageFrameBuffer::init(bgfx::TextureHandle texture) {
    mTextureHandle = texture;
}

bgfx::TextureHandle XImageFrameBuffer::getTexture() {
    return mTextureHandle;
}