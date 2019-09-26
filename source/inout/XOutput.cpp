//
// Created by 林炳河 on 2019/7/14.
//

#include "XOutput.hpp"
#include "XFrameBufferPool.hpp"
#include "XLog.hpp"

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
    // 接收外部目标存储帧数据时
    if (!mOuterBuffer && outputBuffer != nullptr && mOutputFrameBuffer != outputBuffer) {
        XFrameBufferPool::recycle(mOutputFrameBuffer);
        mOutputFrameBuffer = outputBuffer;
        mOuterBuffer = true;
    }
    // 之前接收过外部目标存储帧数据，需要清空重新使用自身目标帧数据存储时
    if (mOuterBuffer && outputBuffer == nullptr && mOutputFrameBuffer != nullptr) {
        mOutputFrameBuffer = nullptr;
        mOuterBuffer = false;
    }
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