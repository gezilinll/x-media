//
// Created by 林炳河 on 2019/7/14.
//

#include "XInput.hpp"

NS_X_IMAGE_BEGIN
XInput::XInput() {
    mWidth = 0;
    mHeight = 0;
}

void XInput::setViewSize(int width, int height) {
    mWidth = width;
    mHeight = height;
}

void XInput::setInputFrameBuffer(XFrameBuffer *input) {

}

void XInput::submit() {

}
NS_X_IMAGE_END