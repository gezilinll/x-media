//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEINPUT_H
#define GPUIMAGE_X_XIMAGEINPUT_H

#include "XImageFrameBuffer.hpp"

NS_X_IMAGE_BEGIN
class XImageInput {
public:
    virtual void newFrameReadyAtProgress(float progress, int index);

    virtual void setInputFrameBuffer(XImageFrameBuffer* input);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEINPUT_H
