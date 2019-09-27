//
// Created by 林炳河 on 2019-09-23.
//

#ifndef GPUIMAGE_X_XFILTEREFFECTLISTUI_HPP
#define GPUIMAGE_X_XFILTEREFFECTLISTUI_HPP

#include "XFilterEffectUI.hpp"
#include "XLayer.hpp"
#include <unordered_map>

NS_X_IMAGE_BEGIN
class XFilterEffectListUI {
public:
    XFilterEffectListUI();

    ~XFilterEffectListUI();

    void imgui(XLayer* layer);

private:
    XEffectUI* getEffectUI(std::string name);

private:
    std::unordered_map<std::string, XEffectUI*> mEffectUIs;
    int mCurrentIndex;
    bool mSaturation;
    bool mContrast;
    bool mBrightness;
    bool mHUE;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECTLISTUI_HPP
