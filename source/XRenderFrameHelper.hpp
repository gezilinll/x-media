//
// Created by 林炳河 on 2019-09-26.
//

#ifndef GPUIMAGE_X_XRENDERFRAMEHELPER_HPP
#define GPUIMAGE_X_XRENDERFRAMEHELPER_HPP

#include "XLayer.hpp"
#include <XModels.hpp>

NS_X_IMAGE_BEGIN
class XRenderFrameHelper {
public:
    static void blend(std::vector<XLayer *> layers, XFrameBuffer* toResult);

    static void overlayEffects(XFrameBuffer *blendResult, std::vector<XEffect *> effects, XFrameBuffer* toResult);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XRENDERFRAMEHELPER_HPP
