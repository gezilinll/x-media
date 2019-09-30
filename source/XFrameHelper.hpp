//
// Created by 林炳河 on 2019-09-26.
//

#ifndef GPUIMAGE_X_XFRAMEHELPER_HPP
#define GPUIMAGE_X_XFRAMEHELPER_HPP

#include "XLayer.hpp"
#include <XModels.hpp>

NS_X_IMAGE_BEGIN
class XFrameHelper {
public:
    static void blendAndEffects(std::vector<XLayer *> layers, std::vector<XEffect *> effects, XFrameBuffer* toResult);

private:
    static void blend(std::vector<XLayer *> layers, XFrameBuffer *toResult);

    static void effect(XFrameBuffer *blendResult, std::vector<XEffect *> effects, XFrameBuffer* toResult);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFRAMEHELPER_HPP
