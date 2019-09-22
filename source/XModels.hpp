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
    int x; /// x开始坐标
    int y; /// y开始坐标
    unsigned int width; /// 矩形宽度
    unsigned int height; /// 矩形高度
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XMODELS_HPP
