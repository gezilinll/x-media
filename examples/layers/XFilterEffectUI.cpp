//
// Created by 林炳河 on 2019-09-23.
//

#include "XFilterEffectUI.hpp"
#include "imgui/imgui.h"

NS_X_IMAGE_BEGIN
XFilterEffectUI::XFilterEffectUI(XImageNS::XFilterEffect *effect) : XEffectUI(effect) {

}

XSaturationUI::XSaturationUI(XSaturation *saturation) : XFilterEffectUI(saturation) {
    XFilterParam param = saturation->getParam("saturation");
    mSaturationValue = param.value[0];
}

void XSaturationUI::imgui() {
    XSaturation* saturation = dynamic_cast<XSaturation *>(mEffect);
    ImGui::Separator();
    ImGui::Text("Saturation");
    XFilterParam param = saturation->getParam("saturation");
    ImGui::SliderFloat(
            ("saturation :" + std::to_string(mIndex)).data()
            , &mSaturationValue
            , param.valueMin[0]
            , param.valueMax[0]
    );
}

void XSaturationUI::update() {
    XSaturation* saturation = dynamic_cast<XSaturation *>(mEffect);
    glm::vec4 value = {mSaturationValue, mSaturationValue, mSaturationValue, mSaturationValue};
    saturation->updateValue("saturation", value);
}

NS_X_IMAGE_END