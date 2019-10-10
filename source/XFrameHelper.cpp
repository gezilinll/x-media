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
    if (layerSize >= 2) {
        XMixer *mixer = layers[1]->getMixer();
        XTwoInputEffectProcessor *processor = dynamic_cast<XTwoInputEffectProcessor*>(mixer->get());
        XTwoInputEffectProcessor *blendChain = processor;
        // 此处必须清空Targets否则在复用时会出现无限叠加Target的情况
        blendChain->clearTargets();
        blendChain->setInputFrameBuffer(layers[0]->get());
        blendChain->setSecondInputFrameBuffer(layers[1]->get());
        blendChain->setViewRect(screen);
        if (layerSize == 2) {
            processor->setOutputBuffer(toResult);
        } else {
            for (int i = 1; i < layerSize - 1; i++) {
                mixer = layers[i + 1]->getMixer();
                XTwoInputEffectProcessor *blend = dynamic_cast<XTwoInputEffectProcessor*>(mixer->get());
                blend->setSecondInputFrameBuffer(layers[i + 1]->get());
                blend->setViewRect(screen);
                blend->clearTargets();
                blendChain->addTarget(blend);
                blendChain = blend;
            }
            // 将最后一次混合输出到结果缓存上
            blendChain->setOutputBuffer(toResult);
        }
        processor->submit();
    } else {
        XFilter *filterEffect = new XFilter(XFilterType::DEFAULT);
        XEffectProcessor *filter = dynamic_cast<XEffectProcessor*>(filterEffect->get());
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
            }
            // 将最后一次特效叠加输出到结果缓存上
            preEffect->setOutputBuffer(toResult);
        }
        dynamic_cast<XInput *>(processor)->submit();
    }
}
NS_X_IMAGE_END