//
// Created by 林炳河 on 2019/7/14.
//

#include "XFrameBuffer.hpp"
#include "XImage.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
XFrameBuffer::XFrameBuffer(bgfx::FrameBufferHandle handle, int width, int height, bool occupied) {
    mHandle = handle;
    mWidth = width;
    mHeight = height;
    mOccupied = occupied;
}

XFrameBuffer::~XFrameBuffer() {
    XImage::destroy(mHandle);
}

void XFrameBuffer::setFrameBufferHandle(bgfx::FrameBufferHandle handle) {
    if (mHandle.idx != handle.idx) {
        XImage::destroy(mHandle);
    }
    mHandle = handle;
}

void XFrameBuffer::setOccupied(bool occupied) {
    mOccupied = occupied;
}

bool XFrameBuffer::isOccupied() {
    return mOccupied;
}

bool XFrameBuffer::isSameSize(int width, int height) {
    return mWidth == width && mHeight == height;
}

bgfx::FrameBufferHandle XFrameBuffer::get() {
    return mHandle;
}

bgfx::TextureHandle XFrameBuffer::getTexture() {
    return bgfx::getTexture(mHandle);
}

NS_X_IMAGE_END
