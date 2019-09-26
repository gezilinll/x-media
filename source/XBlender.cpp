//
// Created by 林炳河 on 2019-09-25.
//

#include "XBlender.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XTwoInputFilter* XBlender::sTwoInputFilter = nullptr;

void XBlender::blend(XFrameBuffer *bottom, XFrameBuffer *top) {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    if (sTwoInputFilter == nullptr) {
        sTwoInputFilter = new XTwoInputFilter("vs_filter_normal", "fs_blend_multiply");
    }
    sTwoInputFilter->setViewRect(screen);
    sTwoInputFilter->setInputFrameBuffer(bottom);
    sTwoInputFilter->setSecondInputFrameBuffer(top);
    sTwoInputFilter->submit();
}
NS_X_IMAGE_END