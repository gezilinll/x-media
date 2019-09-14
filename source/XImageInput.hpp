//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEINPUT_H
#define GPUIMAGE_X_XIMAGEINPUT_H

#include "XImageFrameBuffer.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief XImage's base target object
 *
 * Processing many kind of textures like effects and so on, which are subclasses of XImageInput. These include:
 * - XImageFilter and its subclasses
 */
class XImageInput {
public:
    /**
     * @brief notify new frame is ready and do process
     * @param[in] progress current progress, [0.0, 1.0]
     */
    virtual void renderAtProgress(float progress);

    /**
     * @brief set frame buffer to be processing
     * @param[in] input frame buffer
     */
    virtual void setInputFrameBuffer(XImageFrameBuffer* input);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEINPUT_H
