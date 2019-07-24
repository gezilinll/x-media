//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEOUTPUT_H
#define GPUIMAGE_X_XIMAGEOUTPUT_H

#include "XImageInput.hpp"
#include "XImageFrameBuffer.hpp"
#include <vector>

NS_X_IMAGE_BEGIN
/**
 * @brief XImage's base source object
 *
 * Images or frames of video or other texture type are uploaded from source objects, which are subclasses of XImageOutput. These include:
 * - XImagePictureOutput
 *
 * Source objects upload still image frames to OpenGL ES as textures, then hand those textures off to the next objects in the processing chain.
 */
class XImageOutput {
public:
    XImageOutput();

    ~XImageOutput();

    /**
     * @brief adds a target to receive notifications when new frames are available.
     * The target will be asked for its next available texture.
     * See \link XImageInput::newFrameReadyAtProgress \endlink
     * @param[in] target
     */
    void addTarget(XImageInput* target);

    /**
     * @brief clear all targets but won't delete target pointer
     */
    void clearTarget();

    /**
     * @brief notify target there has new frame and call \link XImageInput::setInputFrameBuffer \endlink
     */
    void notifyTargetsAboutNewOutputTexture();

protected:
    XImageFrameBuffer* mOutputFrameBuffer; /// frame buffer which are uploaded from source
    std::vector<XImageInput *> mTargets; /// targets to receive frame buffer

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEOUTPUT_H
