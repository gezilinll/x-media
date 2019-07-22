//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEOUTPUT_H
#define GPUIMAGE_X_XIMAGEOUTPUT_H

#include "XImageInput.hpp"
#include "XImageFrameBuffer.hpp"
#include <vector>

NS_X_IMAGE_BEGIN
class XImageOutput {
public:
    XImageOutput();

    ~XImageOutput();

    void addTarget(XImageInput* target);
    
    void clearTarget();
    
    void notifyTargetsAboutNewOutputTexture();

protected:
    XImageFrameBuffer* mOutputFrameBuffer;
    std::vector<XImageInput *> mTargets;

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEOUTPUT_H
