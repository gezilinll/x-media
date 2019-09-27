//
// Created by 林炳河 on 2019-09-27.
//

#include "XBlendUI.hpp"
#include "imgui/imgui.h"
#include "bx/bx.h"

NS_X_IMAGE_BEGIN
const char *BLEND_MODES[] = {"Normal", "Multiply", "Add"};

XBlendUI::XBlendUI() {
    mCurrentIndex = 0;
    XBlend normal = {XBlend::Type::NORMAL};
    XBlend multiply = {XBlend::Type::MULTIPLY};
    XBlend add = {XBlend::Type::ADD};
    mBlends.push_back(normal);
    mBlends.push_back(multiply);
    mBlends.push_back(add);
}

void XBlendUI::imgui(XImageNS::XLayer *layer) {
    ImGui::Separator();
    int size = BX_COUNTOF(BLEND_MODES);
    ImGui::Combo("Blend Modes", &mCurrentIndex, BLEND_MODES, size, 5);
    layer->setBlend(mBlends[mCurrentIndex]);
}
NS_X_IMAGE_END