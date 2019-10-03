//
// Created by 林炳河 on 2019/10/2.
//

#include "XMixer.hpp"

NS_X_IMAGE_BEGIN
XMixer::XMixer(XMixerType type) : XFilter() {
    mType = type;

    init();
}

void XMixer::init() {
    if (mFilter == nullptr) {
        if (!initBlend() && !initMatte() && !initTransition()) {
            LOGE("[XMixer] type is error.");
            return;
        }

        if (mFilter == nullptr) {
            mFilter = new XTwoInputShaderProcessor(mVertexShaderName, mFragmentShaderName);
        }
    }

}

bool XMixer::initBlend() {
    bool result = true;
    if (mType == XMixerType::BLEND_NORMAL) {
        mFragmentShaderName ="fs_blend_normal";
        setName("Blend-Normal");
    } else if (mType == XMixerType::BLEND_MULTIPLY) {
        mFragmentShaderName ="fs_blend_multiply";
        setName("Blend-Multiply");
    } else if (mType == XMixerType::BLEND_ADD) {
        mFragmentShaderName ="fs_blend_add";
        setName("Blend-Add");
    } else {
        result = false;
    }
    return result;
}

bool XMixer::initMatte() {
    bool result = true;
    if (mType == XMixerType::MATTE_ALPHA) {
        mFragmentShaderName ="fs_matte_alpha";
        setName("Matte-Alpha");
    } else {
        result = false;
    }
    return result;
}

bool XMixer::initTransition() {
    bool result = true;
    if (mType == XMixerType::TRANSITION_FADE) {
        mFragmentShaderName ="fs_transition_fade";
        addParam("fadeOutOpacity", 0.0f, 1.0f, 0.5f, 0.5f);
        setName("Transition-Fade");
    } else {
        result = false;
    }
    return result;
}
NS_X_IMAGE_END