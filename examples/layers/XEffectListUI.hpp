//
// Created by 林炳河 on 2019-09-23.
//

#ifndef GPUIMAGE_X_XEFFECTLISTUI_HPP
#define GPUIMAGE_X_XEFFECTLISTUI_HPP

#include "XFilterEffectUI.hpp"

NS_X_IMAGE_BEGIN
class XEffectListUI {
public:
    XEffectListUI();

    void imgui();

    XEffectUI* newEffectUI();

private:
    XEffectUI *mNewEffectUI;
    int mCurrentIndex;
    int mLastIndex;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XEFFECTLISTUI_HPP
