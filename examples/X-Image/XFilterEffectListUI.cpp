//
// Created by 林炳河 on 2019-09-23.
//

#include "XFilterEffectListUI.hpp"
#include "imgui/imgui.h"

NS_X_IMAGE_BEGIN
const char *EFFECT_ITEMS[] = {"None", "Saturation", "Contrast", "Brightness", "HUE", "Exposure", "RGB", "WhiteBalance",
                              "Levels", "Monochrome"};

XFilterEffectListUI::XFilterEffectListUI() {
    mCurrentIndex = 0;
    mSaturation = false;
    mContrast = false;
    mBrightness = false;
    mHUE = false;
    mExposure = false;
    mRGB = false;
    mLevels = false;
    mWhiteBalance = false;
    mMonochrome = false;
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
    ImGui::Checkbox("Exposure", &mExposure);
    ImGui::Checkbox("RGB", &mRGB);
    ImGui::Checkbox("WhiteBalance", &mWhiteBalance);
    ImGui::Checkbox("Levels", &mLevels);
    ImGui::Checkbox("Monochrome", &mMonochrome);

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
    if (mExposure) {
        effects.push_back(getEffectUI("Exposure")->getEffect());
        effectNames.push_back("Exposure");
    }
    if (mRGB) {
        effects.push_back(getEffectUI("RGB")->getEffect());
        effectNames.push_back("RGB");
    }
    if (mWhiteBalance) {
        effects.push_back(getEffectUI("WhiteBalance")->getEffect());
        effectNames.push_back("WhiteBalance");
    }
    if (mLevels) {
        effects.push_back(getEffectUI("Levels")->getEffect());
        effectNames.push_back("Levels");
    }
    if (mMonochrome) {
        effects.push_back(getEffectUI("Monochrome")->getEffect());
        effectNames.push_back("Monochrome");
    }
    if (effects.size() == 0) {
        mCurrentIndex = 0;
        effects.push_back(getEffectUI("None")->getEffect());
    }
    if (effectNames.size() <= mCurrentIndex) {
        mCurrentIndex = 0;
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
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::DEFAULT));
        } else if (name == "Saturation") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::SATURATION));
        } else if (name == "Contrast") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::CONTRAST));
        } else if (name == "Brightness") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::BRIGHTNESS));
        } else if (name == "HUE") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::HUE));
        } else if (name == "Exposure") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::EXPOSURE));
        } else if (name == "RGB") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::RGB));
        } else if (name == "WhiteBalance") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::WHITE_BALANCE));
        } else if (name == "Levels") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::LEVELS));
        } else if (name == "Monochrome") {
            effectUi = new XFilterEffectUI(new XFilter(XFilterType::MONOCHROME));
        }
        mEffectUIs.insert(std::make_pair(name, effectUi));
    } else {
        effectUi = iter->second;
    }
    return effectUi;
}

NS_X_IMAGE_END