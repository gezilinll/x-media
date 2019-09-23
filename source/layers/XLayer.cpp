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

void XLayer::setViewRect(XRect &rect) {
    mViewRect = rect;
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
    for (int i = 0; i < size - 1; i++) {
        XInputOutput *current = mEffects[i]->get();
        XInputOutput *next = mEffects[i + 1]->get();
        current->clearTargets();
        next->clearTargets();
        current->setViewRect(mViewRect);
        next->setViewRect(mViewRect);
        current->addTarget(next);
    }
    if (size > 0) {
        XInputOutput *target = mEffects[0]->get();
        target->setViewRect(mViewRect);
        mLayerSource->addTarget(target);
    }

    mLayerSource->submit();
}

NS_X_IMAGE_END