//
// Created by 林炳河 on 2019-09-23.
//

#include "XEffectListUI.hpp"
#include "imgui/imgui.h"

NS_X_IMAGE_BEGIN
const char *EFFECT_ITEMS[] = {"None", "Saturation", "Contrast", "Brightness", "HUE"};

XEffectListUI::XEffectListUI() {
    mNewEffectUI = nullptr;
    mLastIndex = mCurrentIndex = 0;
}

void XEffectListUI::imgui() {
    ImGui::Separator();
    ImGui::Text("Effects:");
    int size = IM_ARRAYSIZE(EFFECT_ITEMS);
    ImGui::Combo("", &mCurrentIndex, EFFECT_ITEMS, size, 5);
    if (mLastIndex != mCurrentIndex) {
        if (EFFECT_ITEMS[mCurrentIndex] == "Saturation") {
            mNewEffectUI = new XFilterEffectUI(new XSaturation());
        } else if (EFFECT_ITEMS[mCurrentIndex] == "Contrast") {
            mNewEffectUI = new XFilterEffectUI(new XContrast());
        } else if (EFFECT_ITEMS[mCurrentIndex] == "Brightness") {
            mNewEffectUI = new XFilterEffectUI(new XBrightness());
        } else if (EFFECT_ITEMS[mCurrentIndex] == "HUE") {
            mNewEffectUI = new XFilterEffectUI(new XHUE());
        } else {
            mNewEffectUI = nullptr;
        }
    } else {
        mNewEffectUI = nullptr;
    }
    mLastIndex = mCurrentIndex;
}

XEffectUI* XEffectListUI::newEffectUI() {
    return mNewEffectUI;
}
NS_X_IMAGE_END