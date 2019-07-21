//
// Created by 林炳河 on 2019/7/14.
//

#include "XImagePictureOutput.hpp"
#include "bigg.hpp"

void XImagePictureOutput::initWithPath(std::string path) {
    mOutputFrameBuffer->loadFromPicture(path);
}

void XImagePictureOutput::processPicture() {
    auto iter = mTargets.begin();
    while (iter != mTargets.end()) {
        XImageInput* input = *iter;
        input->newFrameReadyAtProgress(0, 0);
        iter++;
    }
}
