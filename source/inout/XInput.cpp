//
// Created by 林炳河 on 2019/7/14.
//

#include "XInput.hpp"

NS_X_IMAGE_BEGIN
XInput::XInput() {
    mRect = {0, 0, 0, 0};
}

void XInput::setViewRect(XRect &rect) {
    mRect = rect;
}

XRect XInput::getViewRect() {
    return mRect;
}

void XInput::setInputFrameBuffer(XFrameBuffer *input) {

}

void XInput::submit() {

}
NS_X_IMAGE_END