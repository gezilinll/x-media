//
// Created by 林炳河 on 2019-09-23.
//

#include "XFilterEffectListUI.hpp"
#include "imgui/imgui.h"

NS_X_IMAGE_BEGIN
const char *EFFECT_ITEMS[] = {"None", "Saturation", "Contrast", "Brightness", "HUE"};

XFilterEffectListUI::XFilterEffectListUI() {
    mCurrentIndex = 0;
    mSaturation = false;
    mContrast = false;
    mBrightness = false;
    mHUE = false;
}

XFilterEffectListUI::~XFilterEffectListUI() {
    for (auto entry : mEffectUIs) {
        SAFE_DELETE(entry.second);
    }
     mEffectUIs.clear();
}

void XFilterEffectListUI::imgui(XLayer *layer) {
    ImGui::Separator();
    ImGui::Text("Filter Effects:");
    ImGui::Checkbox("Saturation", &mSaturation);
    ImGui::Checkbox("Contrast", &mContrast);
    ImGui::Checkbox("Brightness", &mBrightness);
    ImGui::Checkbox("HUE", &mHUE);

    std::vector<XEffect*> effects;
    std::vector<char *> effectNames;
    effectNames.push_back("None");
    if (mSaturation) {
        effects.push_back(getEffectUI("Saturation")->getEffect());
        effectNames.push_back("Saturation");
    }
    if (mContrast) {
        effects.push_back(getEffectUI("Contrast")->getEffect());
        effectNames.push_back("Contrast");
    }
    if (mBrightness) {
        effects.push_back(getEffectUI("Brightness")->getEffect());
        effectNames.push_back("Brightness");
    }
    if (mHUE) {
        effects.push_back(getEffectUI("HUE")->getEffect());
        effectNames.push_back("HUE");
    }
    if (effects.size() == 0) {
        effects.push_back(getEffectUI("None")->getEffect());
    }
    layer->setEffects(effects);
    const char *names[effectNames.size()];
    std::copy(effectNames.begin(), effectNames.end(), names);
    ImGui::Combo("Filter Params", &mCurrentIndex, names, effectNames.size(), 5);
    if (mCurrentIndex != 0) {
        getEffectUI(names[mCurrentIndex])->imgui();
        getEffectUI(names[mCurrentIndex])->update();
    }
}

XEffectUI* XFilterEffectListUI::getEffectUI(std::string name) {
    auto iter = mEffectUIs.find(name);
    XEffectUI *effectUi = nullptr;
    if (iter == mEffectUIs.end()) {
        if (name == "None") {
            effectUi = new XFilterEffectUI(new XFilterEffect());
        } else if (name == "Saturation") {
            effectUi = new XFilterEffectUI(new XSaturation());
        } else if (name == "Contrast") {
            effectUi = new XFilterEffectUI(new XContrast());
        } else if (name == "Brightness") {
            effectUi = new XFilterEffectUI(new XBrightness());
        } else if (name == "HUE") {
            effectUi = new XFilterEffectUI(new XHUE());
        }
        mEffectUIs.insert(std::make_pair(name, effectUi));
    } else {
        effectUi = iter->second;
    }
    return effectUi;
}

NS_X_IMAGE_END