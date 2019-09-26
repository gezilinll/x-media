//
// Created by 林炳河 on 2019/7/14.
//

#include "XOutput.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
XOutput::XOutput() {
    mOutputFrameBuffer = nullptr;
    mOuterBuffer = false;
}

XOutput::~XOutput() {
    if (!mOuterBuffer) {
        XFrameBufferPool::recycle(mOutputFrameBuffer);
    }
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

void XOutput::setOutputBuffer(XFrameBuffer *outputBuffer) {
    if (mOutputFrameBuffer != outputBuffer) {
        XFrameBufferPool::recycle(mOutputFrameBuffer);
    }
    mOutputFrameBuffer = outputBuffer;
    mOuterBuffer = true;
}

void XOutput::setOutputSize(int width, int height) {
    mOutputWidth = width;
    mOutputHeight = height;
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