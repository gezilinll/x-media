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
 * @brief 承载图片、视频帧、渲染结果等内容的数据对象，该类会将上述内容存放至FBO中
 *
 * 该类不应自行创建，如要使用必须通过 #XFrameBufferPool 进行获取，当 #isisOccupied 为 TRUE 时，表示该帧数据对象处于被占用的状态
 */
class XFrameBuffer {
public:
    /**
     * @brief 创建数据对象
     * @param handle 帧数据标识
     * @param width 帧数据宽度
     * @param height 帧数据宽度
     * @param occupied 是否被占用，默认为是
     */
    XFrameBuffer(bgfx::FrameBufferHandle handle, int width, int height, bool occupied = true);

    ~XFrameBuffer();

    /**
     * @brief 设置bgfx中的FBO标识
     * @param handle FBO标识
     * @attention 如果与目前的FBO标识不一样，那么会回收之前旧的FBO
     */
    void setFrameBufferHandle(bgfx::FrameBufferHandle handle);

    /**
     * @brief 设置该对象是否被占用
     * @param occupied 是否被占用
     */
    void setOccupied(bool occupied);

    /**
     * @brief 该对象是否被占用
     */
    bool isOccupied();

    /**
     * @brief 该对象的宽高是否与传入的参数一致
     */
    bool isSameSize(int width, int height);

    /**
     * @brief 获取bgfx中的FBO标识
     */
    bgfx::FrameBufferHandle get();

    /**
     * @brief 获取bgfx中该FBO对应的纹理标识
     */
    bgfx::TextureHandle getTexture();

private:
    bgfx::FrameBufferHandle mHandle; /// FBO标识
    int mWidth; /// 宽度
    int mHeight; /// 高度
    bool mOccupied; /// 是否占用
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFRAMEBUFFER_HPP
