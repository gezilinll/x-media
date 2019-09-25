//
// Created by 林炳河 on 2019-09-25.
//

#include "XBlender.hpp"
#include "XImage.hpp"
#include "XTwoInputFilter.hpp"

NS_X_IMAGE_BEGIN
void XBlender::blend(XFrameBuffer *bottom, XFrameBuffer *top) {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    XTwoInputFilter *twoInputFilter = new XTwoInputFilter("vs_filter_normal", "fs_blend_multiply");
    twoInputFilter->setViewRect(screen);
    twoInputFilter->setInputFrameBuffer(bottom);
    twoInputFilter->setSecondInputFrameBuffer(top);
    twoInputFilter->submit();
    SAFE_DELETE(twoInputFilter);
}
NS_X_IMAGE_END