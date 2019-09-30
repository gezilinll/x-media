//
// Created by 林炳河 on 2019/9/14.
//

#ifndef GPUIMAGE_X_XIMAGEINPUTOUTPUT_HPP
#define GPUIMAGE_X_XIMAGEINPUTOUTPUT_HPP

#include "XInput.hpp"
#include "XOutput.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 同时具备处理输入数据与产生输出数据能力的基类
 *
 * 注意：
 * 1. 当未 #setOutputSize 设置输出尺寸时，则会使用 #setViewRect 的宽高来代替输出尺寸
 * 2. 因为该类继承自 #XInput 以及 #XOutput ，两个类都有 #submit 接口，因此当使用该类时无法直接调用，需要选择使用哪个基类的该接口
 */
class XInputOutput: public XInput, public XOutput {

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEINPUTOUTPUT_HPP
