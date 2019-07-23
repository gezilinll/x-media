//
// Created by 林炳河 on 2019/7/21.
//

#ifndef GPUIMAGE_X_XIMAGEUTILS_HPP
#define GPUIMAGE_X_XIMAGEUTILS_HPP

#include "bgfx/bgfx.h"
#include "bimg/bimg.h"
#include "bimg/decode.h"
#include <bx/allocator.h>
#include "bx/file.h"
#include "XMacros.hpp"

NS_X_IMAGE_BEGIN
class XImageUtils {
public:
    static bgfx::TextureHandle
    loadTexture(const char *name, uint64_t flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE, uint8_t skip = 0,
                bgfx::TextureInfo *info = NULL, bimg::Orientation::Enum *orientation = NULL);

    static bgfx::ProgramHandle loadProgram( const char* vsName, const char* fsName );

    static float* wrapFloatToVec4(float value);

    static float* wrapVec3ToVec4(float x, float y, float z);

private:
    static bgfx::TextureHandle
    loadTexture(bx::FileReaderI *reader, const char *filePath, uint64_t flags, uint8_t skip,
                bgfx::TextureInfo *_info, bimg::Orientation::Enum *orientation);

    static bgfx::ProgramHandle loadProgram(bx::FileReaderI* reader, const char* vsName, const char* fsName);

    static bgfx::ShaderHandle loadShader(bx::FileReaderI* reader, const char* name);

    static const bgfx::Memory* loadMem(bx::FileReaderI* reader, const char* filePath);

    static bx::FileReaderI* getFileReader();
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEUTILS_HPP
