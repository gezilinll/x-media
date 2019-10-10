//
// Created by 林炳河 on 2019/10/2.
//

#include "XMixer.hpp"

NS_X_IMAGE_BEGIN
XMixer::XMixer(XMixerType type) : XFilter() {
    mType = type;

    init();
}

bool XMixer::isSame(XMixerType type) {
    return mType == type;
}

void XMixer::init() {
    if (mFilter == nullptr) {
        // 普通混合类型
        if (mType == XMixerType::NORMAL) {
            mFragmentShaderName ="fs_blend_normal";
            setName("Blend-Normal");
        } else if (mType == XMixerType::MULTIPLY) {
            mFragmentShaderName ="fs_blend_multiply";
            setName("Blend-Multiply");
        } else if (mType == XMixerType::ADD) {
            mFragmentShaderName ="fs_blend_add";
            setName("Blend-Add");
        }
        // 抠图类型
        else if (mType == XMixerType::ALPHA) {
            mFragmentShaderName ="fs_matte_alpha";
            setName("Matte-Alpha");
        }
        // 转场类型
        else if (mType == XMixerType::FADE) {
            mFragmentShaderName ="fs_transition_fade";
            addParam("fadeOutOpacity", 0.0f, 1.0f, 0.5f, 0.5f);
            setName("Transition-Fade");
        }
        // 类型错误
        else {
            LOGE("[XMixer] type is error.");
            return;
        }

        if (mFilter == nullptr) {
            mFilter = new XTwoInputEffectProcessor(mVertexShaderName, mFragmentShaderName);
        }
    }

}
NS_X_IMAGE_END