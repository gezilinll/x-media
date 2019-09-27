//
// Created by 林炳河 on 2019-09-23.
//

#include "XFilterEffectUI.hpp"
#include "imgui/imgui.h"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XFilterEffectUI::XFilterEffectUI(XImageNS::XFilterEffect *effect) : XEffectUI(effect) {
    XFilterEffect *filter = dynamic_cast<XFilterEffect *>(mEffect);

    mParams = filter->getParams();
}

void XFilterEffectUI::imgui() {
    for (std::pair<std::string, XFilterParam> value : mParams) {
        std::string &paramName = value.first;
        XFilterParam &param = value.second;
        for (int i = 0; i < value.second.valueNum; i++) {
            ImGui::SliderFloat(
                    (value.first + ":" + getLocationName(i)).data()
                    , &param.value[i]
                    , param.valueMin[i]
                    , param.valueMax[i]
            );
        }
        mParams[paramName] = param;
    }
}

void XFilterEffectUI::update() {
    XFilterEffect *filter = dynamic_cast<XFilterEffect *>(mEffect);
    for (std::pair<std::string, XFilterParam> value : mParams) {
        std::string &paramName = value.first;
        XFilterParam &param = value.second;
        filter->updateValue(value.first, param.value);
    }
}

std::string XFilterEffectUI::getLocationName(int i) {
    switch (i) {
        case 0:
            return "x";
        case 1:
            return "y";
        case 2:
            return "z";
        case 3:
            return "w";
        default:
            return "E";
    }
}

NS_X_IMAGE_END