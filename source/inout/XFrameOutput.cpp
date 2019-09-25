//
// Created by 林炳河 on 2019/7/14.
//

#include "XFrameOutput.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
XFrameOutput::XFrameOutput() : XOutput() {
    mOutputUpdated = false;
    mOutputWidth = 0;
    mOutputHeight = 0;
}

void XFrameOutput::init() {
    if (!mOutputUpdated) {
        mOutputFrameBuffer = XFrameBufferPool::get(mFramePath, mOutputWidth, mOutputHeight);
        mOutputUpdated = true;
    }
}

void XFrameOutput::setPath(std::string path) {
    if (mFramePath != path) {
        mFramePath = path;
        mOutputUpdated = false;
    }
}

void XFrameOutput::setOutputSize(int width, int height) {
    mOutputWidth = width;
    mOutputHeight = height;
}

NS_X_IMAGE_END