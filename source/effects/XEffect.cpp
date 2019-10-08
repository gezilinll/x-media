//
// Created by 林炳河 on 2019/9/21.
//

#include "XEffect.hpp"
NS_X_IMAGE_BEGIN
XEffect::XEffect() {
    mEffectName = "None";
}

XEffect::~XEffect() {

}

XEffectProcessor* XEffect::get() {
    LOGE("[XEffect::process] nothing happened when base class called.");
    return nullptr;
}

std::string XEffect::getName() {
    return mEffectName;
}

void XEffect::setName(std::string name) {
    mEffectName = name;
}
NS_X_IMAGE_END