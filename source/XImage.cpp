//
// Created by 林炳河 on 2019/7/21.
//

#include "XImage.hpp"
#include "XFrameBufferPool.hpp"
#include "XEffectProcessor.hpp"
#include "XFilter.hpp"
#include "XFrameHelper.hpp"

NS_X_IMAGE_BEGIN

int XImage::sRenderIndex = -1;
XFrameBuffer* XImage::sFrame = nullptr;
std::vector<XLayer *> XImage::sLayers;
std::vector<XEffect *> XImage::sGlobalEffects;
bgfx::Init XImage::sInit;

void XImage::init(bgfx::Init &init) {
    sInit = init;
    bgfx::init(init);
}

int XImage::getCanvasWidth() {
    return sInit.resolution.width;
}

int XImage::getCanvasHeight() {
    return sInit.resolution.height;
}

void XImage::begin() {
}

void XImage::addLayer(XLayer *layer) {
    sLayers.push_back(layer);
}

void XImage::addGlobalEffect(XImageNS::XEffect *globalEffect) {
    sGlobalEffects.push_back(globalEffect);
}

void XImage::submit() {
    if (sFrame == nullptr) {
        sFrame = XFrameBufferPool::get(XImage::getCanvasWidth(), XImage::getCanvasHeight());
    }
    std::vector<XLayer *> result = filterSortLayers();
    for (XLayer *layer : result) {
        layer->submit();
    }

    XFrameHelper::blendAndEffects(result, sGlobalEffects, sFrame);
}

void XImage::frame() {
    XRect screen = {0, 0, static_cast<unsigned int>(XImage::getCanvasWidth()),
                    static_cast<unsigned int>(XImage::getCanvasHeight())};
    XFilter *filterEffect = new XFilter(XFilterType::DEFAULT);
    XEffectProcessor *filter = dynamic_cast<XEffectProcessor*>(filterEffect->get());
    filter->setInputFrameBuffer(sFrame);
    filter->setViewRect(screen);
    filter->submit();
    SAFE_DELETE(filterEffect);
}

int XImage::nextRenderIndex() {
    if (sRenderIndex > 150) {

    }
    return ++sRenderIndex;
}

int XImage::currentRenderIndex() {
    return sRenderIndex;
}

void XImage::end() {
    bgfx::frame();
    sRenderIndex = -1;
}

void XImage::shutdown() {
    sLayers.clear();
    sGlobalEffects.clear();
    XFrameBufferPool::recycle(sFrame);
    XFrameBufferPool::destroy();

    sInit = {};
    sRenderIndex = -1;
    sFrame = nullptr;

    bgfx::shutdown();
}

glm::vec4 XImage::wrapFloat(float value) {
    return {value, value, value, value};
}

void XImage::destroy(bgfx::ProgramHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

void XImage::destroy(bgfx::TextureHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

void XImage::destroy(bgfx::FrameBufferHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

void XImage::destroy(bgfx::VertexBufferHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

void XImage::destroy(bgfx::IndexBufferHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

void XImage::destroy(bgfx::UniformHandle &handle) {
    if (handle.idx != bgfx::kInvalidHandle) {
        bgfx::destroy(handle);
        handle.idx = bgfx::kInvalidHandle;
    }
}

std::vector<XLayer *> XImage::filterSortLayers() {
    std::vector<XLayer *> result;
    for (XLayer *layer : sLayers) {
        if (!layer->isMask() && layer->isVisible() && !layer->isExcludeFromBlend()) {
            result.push_back(layer);
        }
    }
    std::sort(result.begin(), result.end(), layerZOrderCompare);
    return result;
}

bool XImage::layerZOrderCompare(XLayer *a, XLayer *b) {
    return a->getZOrder() < b->getZOrder();
}

NS_X_IMAGE_END
