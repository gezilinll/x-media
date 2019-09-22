//
// Created by 林炳河 on 2019/9/20.
//

#ifndef GPUIMAGE_X_XMODELS_HPP
#define GPUIMAGE_X_XMODELS_HPP

#include "XMacros.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 存储矩形区域数据
 */
struct XRect {
    float x; /// x开始坐标
    float y; /// y开始坐标
    float width; /// 矩形宽度
    float height; /// 矩形高度
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XMODELS_HPP
