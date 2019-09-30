//
// Created by 林炳河 on 2019-09-26.
//

#include "XFrameHelper.hpp"
#include "XImage.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN

void XFrameHelper::blendAndEffects(std::vector<XLayer *> layers, std::vector<XEffect *> effects, XFrameBuffer *toResult) {
    if (layers.empty()) {
        LOGE("[XFrameHelper::blendAndEffects] layers is empty.");
        return;
    }

    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    XFrameBuffer *cache = nullptr;
    if (!effects.empty()) {
        cache = XFrameBufferPool::get(screen.width, screen.height);
    } else {
        cache = toResult;
    }

    blend(layers, cache);
    effect(cache, effects, toResult);

    if (cache != toResult) {
        XFrameBufferPool::recycle(cache);
    }
}

void XFrameHelper::blend(std::vector<XLayer *> layers, XFrameBuffer *toResult) {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    // 图层混合
    int layerSize = layers.size();
    if (layerSize > 1) {
        std::vector<XBlender*> blenders;
        XBlender *blender = new XBlender(layers[0]->getBlend());
        XTwoInputFilter *processor = dynamic_cast<XTwoInputFilter*>(blender->get());
        XTwoInputFilter *blendChain = processor;
        blendChain->setInputFrameBuffer(layers[0]->get());
        blendChain->setSecondInputFrameBuffer(layers[1]->get());
        blendChain->setViewRect(screen);
        blenders.push_back(blender);
        if (layerSize == 2) {
            processor->setOutputBuffer(toResult);
        } else {
            for (int i = 1; i < layerSize - 1; i++) {
                blender = new XBlender(layers[i]->getBlend());
                XTwoInputFilter *blend = dynamic_cast<XTwoInputFilter*>(blender->get());
                blend->setSecondInputFrameBuffer(layers[i + 1]->get());
                blend->setViewRect(screen);
                blendChain->addTarget(blend);
                blendChain = blend;
                if (i == layerSize - 2) {
                    blend->setOutputBuffer(toResult);
                }
                blenders.push_back(blender);
            }
        }
        processor->submit();
        for (XBlender *xBlender : blenders) {
            SAFE_DELETE(xBlender);
        }
        blenders.clear();
    } else {
        XFilterEffect *filterEffect = new XFilterEffect();
        XFilter *filter = dynamic_cast<XFilter*>(filterEffect->get());
        filter->setInputFrameBuffer(layers[0]->get());
        filter->setOutputBuffer(toResult);
        filter->setViewRect(screen);
        filter->submit();
        SAFE_DELETE(filterEffect);
    }
}


void XFrameHelper::effect(XFrameBuffer *blendResult, std::vector<XEffect *> effects, XFrameBuffer *toResult) {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    // 叠加全局特效
    int effectSize = effects.size();
    if (effectSize > 0) {
        XInputOutput *processor = effects[0]->get();
        XInputOutput *preEffect = processor;
        preEffect->setInputFrameBuffer(blendResult);
        preEffect->setViewRect(screen);
        if (effectSize == 1) {
            processor->setOutputBuffer(toResult);
        } else {
            for (int i = 1; i < effectSize; i++ ){
                XInputOutput *effect = effects[i]->get();
                effect->setViewRect(screen);
                preEffect->addTarget(effect);
                preEffect = effect;
                if (i == effectSize - 1) {
                    effect->setOutputBuffer(toResult);
                }
            }
        }
        dynamic_cast<XInput *>(processor)->submit();
    }
}
NS_X_IMAGE_END