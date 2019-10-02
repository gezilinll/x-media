//
// Created by 林炳河 on 2019-09-27.
//

#ifndef GPUIMAGE_X_XBLENDUI_HPP
#define GPUIMAGE_X_XBLENDUI_HPP

#include "XLayer.hpp"

NS_X_IMAGE_BEGIN
class XBlendUI {
public:
    XBlendUI();

    void imgui(XLayer *layer);

private:
    int mCurrentIndex;
    std::vector<XMixture> mBlends;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XBLENDUI_HPP
