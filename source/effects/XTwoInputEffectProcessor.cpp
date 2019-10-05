//
// Created by 林炳河 on 2019-09-25.
//

#include "XTwoInputEffectProcessor.hpp"
#include "XLog.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XTwoInputEffectProcessor::XTwoInputEffectProcessor(const std::string vertex, const std::string fragment) : XEffectProcessor(vertex, fragment) {
    mSecondInputFrameBuffer = nullptr;
    mTexture2 = BGFX_INVALID_HANDLE;
}

XTwoInputEffectProcessor::~XTwoInputEffectProcessor() {
    XImage::destroy(mTexture2);
}

void XTwoInputEffectProcessor::setSecondInputFrameBuffer(XFrameBuffer *secondFrame) {
    mSecondInputFrameBuffer = secondFrame;
}

void XTwoInputEffectProcessor::updateParams() {
    XEffectProcessor::updateParams();
    if (!bgfx::isValid(mTexture2)) {
        mTexture2 = bgfx::createUniform("s_texColor1", bgfx::UniformType::Sampler);
    }

    bgfx::setTexture(1, mTexture2, mSecondInputFrameBuffer->getTexture());
}

bool XTwoInputEffectProcessor::isValid() {
    if (mSecondInputFrameBuffer == nullptr || !bgfx::isValid(mSecondInputFrameBuffer->getTexture())) {
        LOGE("[XTwoInputEffectProcessor::submit] second input frame is invalid.");
        return false;
    }
    return XEffectProcessor::isValid();
}
NS_X_IMAGE_END