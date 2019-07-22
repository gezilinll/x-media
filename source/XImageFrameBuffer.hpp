//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
#define GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP

#include "bgfx/bgfx.h"
#include "XMacros.hpp"
#include <string>

NS_X_IMAGE_BEGIN
class XImageFrameBuffer {
public:
    XImageFrameBuffer();

    void loadFromPicture(std::string path);

    bgfx::TextureHandle getTexture();
private:
    bgfx::FrameBufferHandle mHandle;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
