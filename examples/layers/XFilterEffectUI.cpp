//
// Created by 林炳河 on 2019-09-23.
//

#include "XFilterEffectUI.hpp"
#include "imgui/imgui.h"

NS_X_IMAGE_BEGIN
XFilterEffectUI::XFilterEffectUI(XImageNS::XFilterEffect *effect) : XEffectUI(effect) {

}

XSaturationUI::XSaturationUI(XSaturation *saturation) : XFilterEffectUI(saturation) {
    mSaturationValue = saturation->paramSaturationDefault;
}

void XSaturationUI::imgui() {
    XSaturation* saturation = dynamic_cast<XSaturation *>(mEffect);
    ImGui::Separator();
    ImGui::Text("Saturation");
    ImGui::SliderFloat((saturation->paramSaturation + std::to_string(mIndex)).data()
            , &mSaturationValue
            , saturation->paramSaturationMin
            , saturation->paramSaturationMax
    );
}

void XSaturationUI::update() {
    XSaturation* saturation = dynamic_cast<XSaturation *>(mEffect);
    saturation->paramSaturationValue = mSaturationValue;
}

NS_X_IMAGE_END