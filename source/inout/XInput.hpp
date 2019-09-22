//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEINPUT_H
#define GPUIMAGE_X_XIMAGEINPUT_H

#include "XFrameBuffer.hpp"
#include "XModels.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 数据输入系统的基类，可对输入的数据进行特定的处理
 *
 * 继承自该类的子类可对输入的数据进行如特效叠加等处理，并可作为输出系统的Target形成链式和分支效果
 * 重载 #setInputFrameBuffer 来控制外部输入数据的设置逻辑
 * 重载 #submit 来控制数据处理逻辑
 */
class XInput {
public:
    XInput();

    /**
     * @brief 设置输入数据的处理视图大小
     * @param rect 视图大小
     * @attention 宽高必须 > 0
     */
    void setViewRect(XRect &rect);

    /**
     * @brief 设置输入数据
     * @param input 输入数据
     */
    virtual void setInputFrameBuffer(XFrameBuffer* input);

    /**
     * @brief 提交相关数据并进行处理
     * @param index 当前处理视图Index
     * @attention index不可重复，并且需要控制在256以内
     */
    virtual void submit(int index);

protected:
    XRect mViewRect; /// 视图大小
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEINPUT_H
