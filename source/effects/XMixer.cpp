//
// Created by 林炳河 on 2019/10/2.
//

#include "XMixer.hpp"

NS_X_IMAGE_BEGIN
XMixer::XMixer(XMixture mixture) : XFilterEffect() {
    mMixture = mixture;

    if (!setBlendFragment() && !setMatteFragment() && !setTransitionFragment()) {
        LOGE("[XMixer] type is error.");
    }
}

bool XMixer::setBlendFragment() {
    bool result = true;
    if (mMixture == XMixture::BLEND_NORMAL) {
        setFragmentShaderName("fs_blend_normal");
    } else if (mMixture == XMixture::BLEND_MULTIPLY) {
        setFragmentShaderName("fs_blend_multiply");
    } else if (mMixture == XMixture::BLEND_ADD) {
        setFragmentShaderName("fs_blend_add");
    } else {
        result = false;
    }
    return result;
}

bool XMixer::setMatteFragment() {
    bool result = true;
    if (mMixture == XMixture::MATTE_ALPHA) {
        setFragmentShaderName("fs_matte_alpha");
    } else {
        result = false;
    }
    return result;
}

bool XMixer::setTransitionFragment() {
    bool result = true;
    if (mMixture == XMixture::TRANSITION_FADE) {
        setFragmentShaderName("fs_transition_fade");
        addParam("fadeOutOpacity", 0.0f, 1.0f, 0.5f, 0.5f);
    } else {
        result = false;
    }
    return result;
}

void XMixer::init() {
    if (mFilter == nullptr) {
        mFilter = new XTwoInputFilter(mVertexShaderName, mFragmentShaderName);
    }
}
NS_X_IMAGE_END