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
 */
class XInputOutput: public XInput, public XOutput {

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEINPUTOUTPUT_HPP
