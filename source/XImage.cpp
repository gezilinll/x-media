//
// Created by 林炳河 on 2019/7/21.
//

#include "XImage.hpp"

NS_X_IMAGE_BEGIN

int XImage::sRenderIndex = -1;

void XImage::begin() {
}

int XImage::renderIndex() {
    return ++sRenderIndex;
}

void XImage::end() {
    // Advance to next frame. Rendering thread will be kicked to
    // process submitted rendering primitives.
    bgfx::frame();

    sRenderIndex = 0;
}

float* XImage::wrapFloatToVec4(float value) {
    return new float[4]{value, 1.0f, 1.0f, 1.0f};
}

float* XImage::wrapVec3ToVec4(float x, float y, float z) {
    return new float[4]{x, y, z, 1.0f};
}

float* XImage::wrapVec3ToVec4(float *xyz) {
    return new float[4]{xyz[0], xyz[1], xyz[2], 1.0f};
}

void XImage::destroy(bgfx::ProgramHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImage::destroy(bgfx::TextureHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImage::destroy(bgfx::FrameBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImage::destroy(bgfx::VertexBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImage::destroy(bgfx::IndexBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImage::destroy(bgfx::UniformHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

NS_X_IMAGE_END
