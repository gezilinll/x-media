//
// Created by 林炳河 on 2019-09-23.
//

#include "XEffectUI.hpp"
NS_X_IMAGE_BEGIN
XEffectUI::XEffectUI(XEffect *effect, bool recycle) {
    mEffect = effect;
    mAutoRecycle = recycle;
}

XEffectUI::~XEffectUI() {
    if (mAutoRecycle) {
        SAFE_DELETE(mEffect);
    }
}

XEffect* XEffectUI::getEffect() {
    return mEffect;
}

void XEffectUI::imgui() {

}

void XEffectUI::update() {

}

NS_X_IMAGE_END