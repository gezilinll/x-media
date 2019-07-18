//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageOutput.hpp"

XImageOutput::XImageOutput() {
    mOutputFrameBuffer = new XImageFrameBuffer();
}

XImageOutput::~XImageOutput() {
    delete(mOutputFrameBuffer);
    mTargets.clear();
}

void XImageOutput::addTarget(XImageInput *target) {
    mTargets.push_back(target);
}

void XImageOutput::notifyTargetsAboutNewOutputTexture() {
    auto iter = mTargets.begin();
    while (iter != mTargets.end()) {
        XImageInput* input = *iter;
        input->setInputFrameBuffer(mOutputFrameBuffer);
        ++iter;
    }
}
