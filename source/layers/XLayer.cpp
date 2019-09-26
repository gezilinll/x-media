//
// Created by 林炳河 on 2019/9/20.
//

#include "XLayer.hpp"
#include "XImage.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
XLayer::XLayer() {
    XLayer(-1);
}

XLayer::XLayer(int id) {
    mID = id;
    mViewRect = {0, 0, 0, 0};
    mLayerSource = nullptr;
    mBlend = {XBlend::Type::Normal};
}

XLayer::~XLayer() {
    mEffects.clear();
}

int XLayer::getID() {
    return mID;
}

void XLayer::addEffect(XEffect *effect) {
    mEffects.push_back(effect);
}

void XLayer::setBlend(XBlend blend) {
    mBlend = blend;
}

XBlend XLayer::getBlend() {
    return mBlend;
}
void XLayer::setViewRect(XRect &rect) {
    mViewRect = rect;
}

XRect XLayer::getViewRect() {
    return mViewRect;
}

void XLayer::updateSource() {
    LOGE("[XLayer::updateSource] nothing happened when base class called.");
}

void XLayer::submit() {
    updateSource();
    if (mLayerSource == nullptr) {
        LOGE("[XLayer::submit] layer source is nullptr. id=%d", mID);
        return;
    }

    mLayerSource->clearTargets();
    int size = mEffects.size();
    if (size > 0) {
        XRect rect = {0, 0, mViewRect.width, mViewRect.height};
        XInputOutput *target = mEffects[0]->get();
        target->setViewRect(rect);
        target->setToBuffer(true);
        mLayerSource->addTarget(target);
        for (int i = 0; i < size - 1; i++) {
            XInputOutput *current = mEffects[i]->get();
            XInputOutput *next = mEffects[i + 1]->get();
            current->clearTargets();
            next->clearTargets();
            current->addTarget(next);

            current->setViewRect(rect);
            current->setOutputSize(mViewRect.width, mViewRect.height);
            current->setToBuffer(true);
            next->setViewRect(rect);
            next->setToBuffer(true);
            next->setOutputSize(mViewRect.width, mViewRect.height);
        }
        mEffects[size - 1]->get()->setOutputSize(XImage::getCanvasWidth(), XImage::getCanvasHeight());
        mEffects[size - 1]->get()->setViewRect(mViewRect);
    }

    mLayerSource->submit();
}

XFrameBuffer* XLayer::get() {
    int size = mEffects.size();
    return mEffects[size - 1]->get()->get();
}

NS_X_IMAGE_END