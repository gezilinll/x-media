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
    XMixture normal = XMixture::BLEND_NORMAL;
    XMixture multiply = XMixture::BLEND_MULTIPLY;
    XMixture add = XMixture::BLEND_ADD;
    XMixture fade = XMixture::TRANSITION_FADE;
    mBlends.push_back(normal);
    mBlends.push_back(multiply);
    mBlends.push_back(add);
    mBlends.push_back(fade);

    mLastMatteIndex = 0;
    mMatteLayer = nullptr;
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
        XMixer *mixer = new XMixer(mBlends[mCurrentBlendIndex]);
        layer->setMixer(mixer);
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
        layer->clearMattes();
        SAFE_DELETE(mMatteLayer);
        if (MATTE_MODES[mCurrentMatteIndex] == "Alpha") {
            mMatteLayer = new XFrameLayer(999);
            mMatteLayer->setMixer(new XMixer(XMixture::MATTE_ALPHA));
            mMatteLayer->setPath("images/mask_alpha.png");
            layer->addMatte(mMatteLayer);
        }
        mLastMatteIndex = mCurrentMatteIndex;
    }
}

XEffectUI * XMixtureUI::getEffectUI(std::string name, XMixer *mixer) {
    auto iter = mEffectUIs.find(name);
    XEffectUI *effectUi = nullptr;
    if (iter == mEffectUIs.end()) {
        if (name == "T-Fade") {
            XFilterEffect *filterEffect = dynamic_cast<XFilterEffect *>(mixer);
            effectUi = new XFilterEffectUI(filterEffect);
        }
        mEffectUIs.insert(std::make_pair(name, effectUi));
    } else {
        effectUi = iter->second;
    }
    return effectUi;
}
NS_X_IMAGE_END