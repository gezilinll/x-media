//
// Created by 林炳河 on 2019-09-26.
//

#include "XBlendHelper.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN

void XBlendHelper::blend(std::vector<XLayer *> layers, XFrameBuffer* toResult) {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};

    XBlender *blender1 = new XBlender(layers[0]->getBlend());
    XBlender *blender2 = new XBlender(layers[1]->getBlend());
    XBlender *blender3 = new XBlender(layers[2]->getBlend());
    XBlender *blender4 = new XBlender(layers[3]->getBlend());
    XTwoInputFilter *blendChain = dynamic_cast<XTwoInputFilter*>(blender1->get());
    blendChain->setInputFrameBuffer(layers[0]->get());
    blendChain->setSecondInputFrameBuffer(layers[1]->get());
    blendChain->setViewRect(screen);
    XTwoInputFilter *blendChain2 = dynamic_cast<XTwoInputFilter*>(blender2->get());
    blendChain2->setSecondInputFrameBuffer(layers[2]->get());
    blendChain2->setViewRect(screen);
    XTwoInputFilter *blendChain3 = dynamic_cast<XTwoInputFilter*>(blender3->get());
    blendChain3->setSecondInputFrameBuffer(layers[3]->get());
    blendChain3->setViewRect(screen);
    XTwoInputFilter *blendChain4 = dynamic_cast<XTwoInputFilter*>(blender4->get());
    blendChain4->setSecondInputFrameBuffer(layers[4]->get());
    blendChain4->setViewRect(screen);
    blendChain4->setOutputBuffer(toResult);
    blendChain->addTarget(blendChain2);
    blendChain2->addTarget(blendChain3);
    blendChain3->addTarget(blendChain4);
    blendChain->submit();
    SAFE_DELETE(blender1);
    SAFE_DELETE(blender2);
    SAFE_DELETE(blender3);
    SAFE_DELETE(blender4);
}

NS_X_IMAGE_END