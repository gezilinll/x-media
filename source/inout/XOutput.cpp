//
// Created by 林炳河 on 2019/7/14.
//

#include "XOutput.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
XOutput::XOutput() {
    mOutputFrameBuffer = nullptr;
    mToBuffer = false;
}

XOutput::~XOutput() {
    XFrameBufferPool::recycle(mOutputFrameBuffer);
    mTargets.clear();
}

void XOutput::init() {
}

void XOutput::addTarget(XInput *target) {
    mTargets.push_back(target);
}

void XOutput::clearTargets() {
    mTargets.clear();
}

void XOutput::setToBuffer(bool toBuffer) {
    mToBuffer = toBuffer;
}

XFrameBuffer* XOutput::get() {
    return mOutputFrameBuffer;
}

void XOutput::submit() {
    init();

    auto iter = mTargets.begin();
    while (iter != mTargets.end()) {
        XInput* input = *iter;
        input->setInputFrameBuffer(mOutputFrameBuffer);
        input->submit();
        ++iter;
    }
}

NS_X_IMAGE_END