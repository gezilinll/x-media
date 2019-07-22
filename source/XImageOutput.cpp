//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageOutput.hpp"
#include "XMacros.hpp"

NS_X_IMAGE_BEGIN
XImageOutput::XImageOutput() {
    mOutputFrameBuffer = new XImageFrameBuffer();
}

XImageOutput::~XImageOutput() {
    SAFE_DELETE(mOutputFrameBuffer);
    mTargets.clear();
}

void XImageOutput::addTarget(XImageInput *target) {
    mTargets.push_back(target);
}

void XImageOutput::clearTarget() {
    mTargets.clear();
}

void XImageOutput::notifyTargetsAboutNewOutputTexture() {
    auto iter = mTargets.begin();
    while (iter != mTargets.end()) {
        XImageInput* input = *iter;
        input->setInputFrameBuffer(mOutputFrameBuffer);
        ++iter;
    }
}
NS_X_IMAGE_END