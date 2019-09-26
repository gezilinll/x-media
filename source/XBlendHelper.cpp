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
    blendChain3->setOutputBuffer(toResult);
    blendChain->addTarget(blendChain2);
    blendChain2->addTarget(blendChain3);
    blendChain->submit();


//    std::vector<XBlender *> blenders;
//    XBlender *blender = new XBlender(layers[0]->getBlend());
//    XTwoInputFilter *blendChain = dynamic_cast<XTwoInputFilter*>(blender->get());
//    blendChain->setInputFrameBuffer(layers[0]->get());
//    blendChain->setSecondInputFrameBuffer(layers[1]->get());
//    blendChain->setViewRect(screen);
//    XTwoInputFilter *lastOutput = blendChain;
//    blenders.push_back(blender);
//
//    int size = layers.size();
//    for (int index = 1; index < size - 1; index++) {
//        XBlender *nextBlend = new XBlender(layers[index]->getBlend());
//        XTwoInputFilter *nextOutput = dynamic_cast<XTwoInputFilter*>(nextBlend->get());
//        nextOutput->setViewRect(screen);
//        nextOutput->setSecondInputFrameBuffer(layers[index]->get());
//        lastOutput->addTarget(nextOutput);
//        lastOutput = nextOutput;
//        blenders.push_back(nextBlend);
//        if (index == size - 2) {
//            nextOutput->setOutputBuffer(toResult);
//        }
//    }
//    blendChain->submit();

//    for (XBlender *blender : blenders) {
//        SAFE_DELETE(blender);
//    }
}

NS_X_IMAGE_END