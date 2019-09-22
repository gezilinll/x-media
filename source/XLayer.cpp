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
    int renderIndex = XImage::renderIndex();
    updateSource();
    if (mLayerSource == nullptr) {
        LOGE("[XLayer::submit] layer source is nullptr. id=%d, renderIndex=%d", mID, renderIndex);
        return;
    }

    mLayerSource->clearTargets();
    for (XEffect *effect : mEffects) {
        XInputOutput *target = effect->process();
        target->setViewRect(mViewRect);
        mLayerSource->addTarget(target);
    }

    mLayerSource->submit(renderIndex);
}

NS_X_IMAGE_END