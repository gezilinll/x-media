//
// Created by 林炳河 on 2019/7/14.
//

#include "XInput.hpp"

NS_X_IMAGE_BEGIN
XInput::XInput() {
    mViewRect = {0, 0, 0, 0};
}

void XInput::setViewRect(XRect &rect) {
    mViewRect = rect;
}

void XInput::setInputFrameBuffer(XFrameBuffer *input) {

}

void XInput::submit(int index) {

}
NS_X_IMAGE_END