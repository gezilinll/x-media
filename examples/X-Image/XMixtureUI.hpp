//
// Created by 林炳河 on 2019-09-27.
//

#ifndef GPUIMAGE_X_XBLENDUI_HPP
#define GPUIMAGE_X_XBLENDUI_HPP

#include "XFrameLayer.hpp"
#include "XEffectUI.hpp"

NS_X_IMAGE_BEGIN
class XMixtureUI {
public:
    XMixtureUI();

    ~XMixtureUI();

    void imgui(XLayer *layer);

private:
    XEffectUI* getEffectUI(std::string name, XMixer *mixer);

private:
    int mCurrentBlendIndex;
    int mLastBlendIndex;
    std::vector<XMixerType> mBlends;

    int mCurrentMatteIndex;
    int mLastMatteIndex;
    XFrameLayer *mMatteLayer;

    std::unordered_map<std::string, XEffectUI*> mEffectUIs;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XBLENDUI_HPP
