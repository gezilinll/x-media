//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
#define GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP

#include "bgfx/bgfx.h"
#include "XMacros.hpp"
#include <string>

NS_X_IMAGE_BEGIN
/**
 * @brief Buffer that store image and frame of video uploaded as FBO and texture or result of render
 */
class XImageFrameBuffer {
public:
    XImageFrameBuffer();

    /**
     * @brief load picture to FBO
     * @param[in] path picture's absolute path
     */
    void loadFromPicture(std::string path);

    /**
     * @brief get texture of this frame
     * @return texture of this frame
     */
    bgfx::TextureHandle getTexture();
private:
    bgfx::FrameBufferHandle mHandle; /// handle of FBO
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
