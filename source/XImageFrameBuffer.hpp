//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
#define GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP

#include "bgfx/bgfx.h"

class XImageFrameBuffer {
public:
    void init(bgfx::TextureHandle texture);

    bgfx::TextureHandle getTexture();
private:
    bgfx::TextureHandle mTextureHandle;
};


#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
