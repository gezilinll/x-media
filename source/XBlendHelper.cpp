//
// Created by 林炳河 on 2019-09-26.
//

#include "XBlendHelper.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN

void XBlendHelper::blend(std::vector<XLayer *> layers, XFrameBuffer* result) {
    int size = layers.size();
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    XFrameBuffer *bottom = layers[0]->get();
    for (int index = 0; index < size - 1; index++) {
        bool isLast = index == size - 2;
        XBlend blend = layers[index]->getBlend();
        XBlender *blender = new XBlender(blend);
        XTwoInputFilter *twoInputFilter = dynamic_cast<XTwoInputFilter*>(blender->get());
        twoInputFilter->setToBuffer(!isLast);
        twoInputFilter->setViewRect(screen);
        twoInputFilter->setInputFrameBuffer(bottom);
        twoInputFilter->setSecondInputFrameBuffer(layers[index + 1]->get());
        twoInputFilter->submit();
        bottom = twoInputFilter->get();
        SAFE_DELETE(blender);
    }
}

NS_X_IMAGE_END