//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
#define GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP

#include "bgfx/bgfx.h"
#include <string>

class XImageFrameBuffer {
public:
    XImageFrameBuffer();

    void loadFromPicture(std::string path);

    bgfx::TextureHandle getTexture();
private:
    bgfx::FrameBufferHandle mHandle;
};


#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
