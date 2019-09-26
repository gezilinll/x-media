//
// Created by 林炳河 on 2019-09-26.
//

#ifndef GPUIMAGE_X_XBLENDHELPER_HPP
#define GPUIMAGE_X_XBLENDHELPER_HPP

#include "XLayer.hpp"
#include <XModels.hpp>

NS_X_IMAGE_BEGIN
class XBlendHelper {
public:
    static void blend(std::vector<XLayer *> layers, XFrameBuffer* result);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XBLENDHELPER_HPP
