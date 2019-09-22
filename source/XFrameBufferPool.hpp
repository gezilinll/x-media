//
// Created by 林炳河 on 2019/9/22.
//

#ifndef GPUIMAGE_X_XFRAMEBUFFERPOOL_HPP
#define GPUIMAGE_X_XFRAMEBUFFERPOOL_HPP

#include "XFrameBuffer.hpp"
#include <vector>

NS_X_IMAGE_BEGIN
/**
 * @brief #XFrameBuffer 的创建、缓存、释放等逻辑控制的工具类，所有 #XFrameBuffer 的获取、回收等逻辑都必须通过该类进行
 */
class XFrameBufferPool {
public:
    /**
     * @brief 获取对应宽高的帧数据对象
     * @param width 宽度，必须大于0
     * @param height 高度，必须大于0
     * @return 帧数据对象
     * @attention 当不再使用该帧数据对象时，必须通过 #recycle 进行回收
     */
    static XFrameBuffer* get(int width, int height);

    /**
     * @brief 获取承载对应图片路径数据的帧对象
     * @param path 图片路径
     * @return 帧对象
     * @attention 当不再使用该帧数据对象时，必须通过 #recycle 进行回收
     */
    static XFrameBuffer* get(std::string path);

    /**
     * @brief 回收帧数据对象
     * @param buffer 帧数据对象
     */
    static void recycle(XFrameBuffer* buffer);

private:
    /**
     * @brief 从缓存池里获取符合宽高并非占用状态的帧数据对象
     * @param width 宽度，必须大于0
     * @param height 高度，必须大于0
     * @return 帧数据对象，当无符合条件的帧数据对象时返回nullptr
     */
    static XFrameBuffer* getFromPool(int width, int height);

private:
    static std::vector<XFrameBuffer *> sBuffers; /// 缓存的帧数据对象列表
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFRAMEBUFFERPOOL_HPP
