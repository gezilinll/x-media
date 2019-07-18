//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEOUTPUT_H
#define GPUIMAGE_X_XIMAGEOUTPUT_H

#include "XImageInput.hpp"
#include "XImageFrameBuffer.hpp"
#include <vector>

class XImageOutput {
public:
    XImageOutput();

    ~XImageOutput();

    void addTarget(XImageInput* target);
    void notifyTargetsAboutNewOutputTexture();

protected:
    XImageFrameBuffer* mOutputFrameBuffer;
    std::vector<XImageInput *> mTargets;

};


#endif //GPUIMAGE_X_XIMAGEOUTPUT_H
