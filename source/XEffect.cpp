//
// Created by 林炳河 on 2019/9/21.
//

#include "XEffect.hpp"
NS_X_IMAGE_BEGIN
XEffect::XEffect() {

}

XEffect::~XEffect() {

}

XInputOutput* XEffect::get() {
    LOGE("[XEffect::process] nothing happened when base class called.");
    return nullptr;
}
NS_X_IMAGE_END