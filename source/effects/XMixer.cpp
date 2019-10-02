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
    return false;
}

bool XMixer::setTransitionFragment() {
    return false;
}

void XMixer::init() {
    if (mFilter == nullptr) {
        mFilter = new XTwoInputFilter(mVertexShaderName, mFragmentShaderName);
    }
}
NS_X_IMAGE_END