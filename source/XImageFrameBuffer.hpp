//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
#define GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"
#include "XMacros.hpp"
#include <string>

NS_X_IMAGE_BEGIN
/**
 * @brief Buffer that store image and frame of video uploaded as FBO and texture or result of render
 */
class XImageFrameBuffer {
public:
    XImageFrameBuffer();

    ~XImageFrameBuffer();

    /**
     * @brief load picture to FBO
     * @param[in] path picture's absolute path
     */
    void loadFromImage(std::string path);

    void create(uint16_t width, uint16_t height, bgfx::TextureFormat::Enum format,
                uint64_t textureFlags = BGFX_SAMPLER_U_CLAMP|BGFX_SAMPLER_V_CLAMP);

    /**
     * @brief get image container from image path
     * @param dstFormat target format
     * @return image container
     * @attention call this method after #loadFromImage has called
     */
    bimg::ImageContainer *getImageContainer(bgfx::TextureFormat::Enum dstFromat);

    /**
     * @brief get texture of this frame
     * @return texture of this frame
     */
    bgfx::TextureHandle getTexture();

    bgfx::FrameBufferHandle get();
private:
    bgfx::FrameBufferHandle mHandle; /// handle of FBO
    std::string mImagePath;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
