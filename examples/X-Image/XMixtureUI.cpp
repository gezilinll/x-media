//
// Created by 林炳河 on 2019-09-27.
//

#include "XMixtureUI.hpp"
#include "XFilterEffectUI.hpp"
#include "imgui/imgui.h"
#include "bx/bx.h"

NS_X_IMAGE_BEGIN
const char *BLEND_TRANSITION_MODES[] = {"Normal", "B-Multiply", "B-Add", "T-Fade"};
const char *MATTE_MODES[] = {"None", "Alpha"};

XMixtureUI::XMixtureUI() {
    mCurrentBlendIndex = 0;
    mLastBlendIndex = 0;
    mCurrentMatteIndex = 0;
    XMixerType normal = XMixerType::BLEND_NORMAL;
    XMixerType multiply = XMixerType::BLEND_MULTIPLY;
    XMixerType add = XMixerType::BLEND_ADD;
    XMixerType fade = XMixerType::TRANSITION_FADE;
    mBlends.push_back(normal);
    mBlends.push_back(multiply);
    mBlends.push_back(add);
    mBlends.push_back(fade);

    mLastMatteIndex = 0;
    mMatteLayer = nullptr;
    mMatteOutput = nullptr;
}

XMixtureUI::~XMixtureUI() {
    SAFE_DELETE(mMatteLayer);

    mBlends.clear();
}

void XMixtureUI::imgui(XLayer *layer) {
    ImGui::Separator();
    int blendSize = BX_COUNTOF(BLEND_TRANSITION_MODES);
    ImGui::Combo("Blend/Transition Modes", &mCurrentBlendIndex, BLEND_TRANSITION_MODES, blendSize, 3);
    if (mLastBlendIndex != mCurrentBlendIndex) {
        layer->setMixer(mBlends[mCurrentBlendIndex]);
        mLastBlendIndex = mCurrentBlendIndex;
    }
    if (std::string(BLEND_TRANSITION_MODES[mCurrentBlendIndex]).find("T-") == 0) {
        XEffectUI *effectUI = getEffectUI(BLEND_TRANSITION_MODES[mCurrentBlendIndex], layer->getMixer());
        effectUI->imgui();
        effectUI->update();
    }

    int matteSize = BX_COUNTOF(MATTE_MODES);
    ImGui::Combo("Matte", &mCurrentMatteIndex, MATTE_MODES, matteSize, 3);
    if (mLastMatteIndex != mCurrentMatteIndex) {
        layer->clearMasks();
        SAFE_DELETE(mMatteLayer);
        SAFE_DELETE(mMatteOutput);
        if (MATTE_MODES[mCurrentMatteIndex] == "Alpha") {
            mMatteLayer = new XLayer(999);
            mMatteOutput = new XFrameOutput();
            mMatteLayer->setMixer(XMixerType::MATTE_ALPHA);
            mMatteOutput->setPath("images/mask_alpha.png");
            mMatteLayer->setSource(mMatteOutput);
            layer->addMask(mMatteLayer);
        }
        mLastMatteIndex = mCurrentMatteIndex;
    }
}

XEffectUI * XMixtureUI::getEffectUI(std::string name, XMixer *mixer) {
    auto iter = mEffectUIs.find(name);
    XEffectUI *effectUi = nullptr;
    if (iter == mEffectUIs.end()) {
        if (name == "T-Fade") {
            XFilter *filterEffect = dynamic_cast<XFilter *>(mixer);
            effectUi = new XFilterEffectUI(filterEffect, false);
        }
        mEffectUIs.insert(std::make_pair(name, effectUi));
    } else {
        effectUi = iter->second;
    }
    return effectUi;
}
NS_X_IMAGE_END