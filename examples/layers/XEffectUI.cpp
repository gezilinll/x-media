//
// Created by 林炳河 on 2019-09-23.
//

#include "XEffectUI.hpp"
NS_X_IMAGE_BEGIN
XEffectUI::XEffectUI(XEffect *effect) {
    mEffect = effect;
    mIndex = 0;
}

void XEffectUI::setIndex(int index) {
    mIndex = index;
}

XEffect* XEffectUI::getEffect() {
    return mEffect;
}

void XEffectUI::imgui() {

}

void XEffectUI::update() {

}

NS_X_IMAGE_END