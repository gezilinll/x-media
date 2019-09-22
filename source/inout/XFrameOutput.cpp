//
// Created by 林炳河 on 2019/7/14.
//

#include "XFrameOutput.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
XFrameOutput::XFrameOutput() : XOutput() {
    mOutputUpdated = false;
}

void XFrameOutput::init() {
    if (!mOutputUpdated) {
        mOutputFrameBuffer = XFrameBufferPool::get(mFramePath);
        mOutputUpdated = true;
    }
}

void XFrameOutput::setPath(std::string path) {
    if (mFramePath != path) {
        mFramePath = path;
        mOutputUpdated = false;
    }
}

NS_X_IMAGE_END