//
// Created by 林炳河 on 2019/9/20.
//

#include "XLayer.hpp"
#include "XImage.hpp"
#include "XLog.hpp"
#include "XFrameBufferPool.hpp"

NS_X_IMAGE_BEGIN
XLayer::XLayer() {
    XLayer(-1);
}

XLayer::XLayer(int id) {
    mID = id;
    mViewRect = {0, 0, 0, 0};
    mLayerSource = nullptr;
    mMixer = new XMixer(XMixerType::BLEND_NORMAL);
    mLayerResult = nullptr;
    mMatte = nullptr;
    mIsVisible = true;
    mZOrder = 0;
}

XLayer::~XLayer() {
    mEffects.clear();
    SAFE_DELETE(mMixer);
    XFrameBufferPool::recycle(mLayerResult);
}

int XLayer::getID() {
    return mID;
}

void XLayer::setSource(XOutput *source) {
    mLayerSource = source;
}

void XLayer::addEffect(XEffect *effect) {
    XMixer *mixer = dynamic_cast<XMixer *>(effect);
    if (mixer) {
        LOGE("[XLayer::addEffect] XMixer can not be added as effect.");
        return;
    }
    mEffects.push_back(effect);
}

void XLayer::setEffects(std::vector<XImageNS::XEffect *> effects) {
    mEffects.clear();
    mEffects = effects;
}

void XLayer::clearEffects() {
    mEffects.clear();
}

std::vector<XEffect *> XLayer::getEffects() {
    return mEffects;
}

void XLayer::setMixer(XMixerType type) {
    if (mMixer->isSame(type)) {
        return;
    }
    SAFE_DELETE(mMixer);
    mMixer = new XMixer(type);
}

void XLayer::updateMixerValue(std::string name, glm::vec4 value) {
    mMixer->updateValue(name, value);
}

XMixer* XLayer::getMixer() {
    return mMixer;
}

void XLayer::addMask(XLayer *mask) {
    mMasks.push_back(mask);
}

void XLayer::clearMasks() {
    mMasks.clear();
}

void XLayer::setMatte(XLayer *matte) {
    mMatte = matte;
}

void XLayer::setVisibility(bool isVisible) {
    mIsVisible = isVisible;
}

bool XLayer::isVisible() {
    return mIsVisible;
}

void XLayer::setZOrder(int zOrder) {
    mZOrder = zOrder;
}

int XLayer::getZOrder() {
    return mZOrder;
}

void XLayer::setViewRect(XRect &rect) {
    mViewRect = rect;
}

XRect XLayer::getViewRect() {
    return mViewRect;
}

void XLayer::submit() {
    if (mLayerSource == nullptr) {
        LOGE("[XLayer::submit] layer source is nullptr. id=%d", mID);
        return;
    }

    mLayerSource->clearTargets();
    int effectSize = mEffects.size();
    int matteSize = mMattes.size();
    if (effectSize == 0 && matteSize == 0) {
        mLayerSource->submit();
        return;
    }

    if (mLayerResult == nullptr) {
        mLayerResult = XFrameBufferPool::get(XImage::getCanvasWidth(), XImage::getCanvasHeight());
    }

    XRect rect = {0, 0, mViewRect.width, mViewRect.height};
    XOutput *chain = mLayerSource;
    // 先进行抠图处理
    if (matteSize > 0) {
        // 先进行遮罩图层的资源初始化和渲染
        for (XLayer *matte: mMattes) {
            matte->clearEffects();
            matte->clearMasks();
            matte->submit();
        }
        XMixer *mixer = mMattes[0]->getMixer();
        XTwoInputEffectProcessor *matteChain = dynamic_cast<XTwoInputEffectProcessor*>(mixer->get());
        // 此处必须清空Targets否则在复用时会出现无限叠加Target的情况
        matteChain->clearTargets();
        matteChain->setSecondInputFrameBuffer(mMattes[0]->get());
        matteChain->setViewRect(rect);
        chain->addTarget(matteChain);
        chain = matteChain;
        for (int i = 1; i < matteSize - 1; i++) {
            mixer =  mMattes[i]->getMixer();
            matteChain = dynamic_cast<XTwoInputEffectProcessor*>(mixer->get());
            matteChain->setSecondInputFrameBuffer(mMattes[i]->get());
            matteChain->setViewRect(rect);
            matteChain->clearTargets();
            chain->addTarget(matteChain);
            chain = matteChain;
        }
        if (effectSize == 0) { // 特效为空时直接将遮罩混合结果输出到图层缓存结果帧上
            chain->setOutputBuffer(mLayerResult);
            chain->setOutputSize(XImage::getCanvasWidth(), XImage::getCanvasHeight());
            dynamic_cast<XTwoInputEffectProcessor *>(chain)->setViewRect(mViewRect);
        }
    }
    // 特效叠加处理
    if (effectSize > 0) {
        XInputOutput *target = mEffects[0]->get();
        target->setViewRect(rect);
        chain->addTarget(target);
        for (int i = 0; i < effectSize - 1; i++) {
            XInputOutput *current = mEffects[i]->get();
            XInputOutput *next = mEffects[i + 1]->get();
            current->clearTargets();
            next->clearTargets();
            current->addTarget(next);

            current->setOutputBuffer(nullptr);
            current->setViewRect(rect);
            current->setOutputSize(mViewRect.width, mViewRect.height);
            next->setOutputBuffer(nullptr);
            next->setViewRect(rect);
            next->setOutputSize(mViewRect.width, mViewRect.height);
        }

        mEffects[effectSize - 1]->get()->setOutputBuffer(mLayerResult);
        mEffects[effectSize - 1]->get()->setOutputSize(XImage::getCanvasWidth(), XImage::getCanvasHeight());
        mEffects[effectSize - 1]->get()->setViewRect(mViewRect);
    }

    if (mMatte != nullptr) {
        mMatte->submit();

    }

    mLayerSource->submit();
}

XFrameBuffer* XLayer::get() {
    if (mEffects.empty() && mMattes.empty()) {
        return mLayerSource->get();
    }
    return mLayerResult;
}
NS_X_IMAGE_END