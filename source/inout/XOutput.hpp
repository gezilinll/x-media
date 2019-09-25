//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEOUTPUT_H
#define GPUIMAGE_X_XIMAGEOUTPUT_H

#include "XInput.hpp"
#include "XFrameBuffer.hpp"
#include <vector>

NS_X_IMAGE_BEGIN
/**
 * @brief 输出系统基类，可控制各种类型数据的输出，并在此基础上叠加输入系统
 *
 * 图片或视频帧或其他需要输出自身处理结果的，都可通过继承自该类来实现，而输出系统会将自身的输出作为输入系统Target的输入
 * 重载 #init 可实现不同输出系统的数据初始化操作和控制是否重新初始化等逻辑
 */
class XOutput {
public:
    XOutput();

    virtual ~XOutput();

    /**
     * @brief 添加数据输出的目标
     * @param target 目标系统
     * @attention 通过该接口添加的Target为并行关系，并非链式组装
     * @attention 添加的输入系统需要由外部进行内存管理
     */
    void addTarget(XInput* target);

    /**
     * @brief 清空所有目标系统
     */
    void clearTargets();

    /**
     * @brief 设置是否将结果强制存到帧对象中
     * @param toBuffer 是否存到帧对象中
     * @note 当Output具有target或者 toBuffer 为 TRUE 时都会将结果存到帧对象中
     */
    void setToBuffer(bool toBuffer);

    /**
     * @brief 获取输出帧数据
     * @return 帧数据
     */
    XFrameBuffer *get();

    /**
     * @brief 提交相关数据并进行处理
     */
    void submit();

protected:
    /**
     * @brief 初始化接口，可在该接口进行数据加载、初始绘制等逻辑
     */
    virtual void init();

protected:
    XFrameBuffer* mOutputFrameBuffer; /// 输出数据Buffer
    std::vector<XInput *> mTargets; /// 接收输出数据的目标列表
    bool mToBuffer; /// 是否强制将结果存到帧数据中
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEOUTPUT_H
