//
// Created by 林炳河 on 2019-09-25.
//

#include "XTwoInputShaderProcessor.hpp"
#include "XLog.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XTwoInputShaderProcessor::XTwoInputShaderProcessor(const std::string vertex, const std::string fragment) : XShaderProcessor(vertex, fragment) {
    mSecondInputFrameBuffer = nullptr;
    mTexture2 = BGFX_INVALID_HANDLE;
}

XTwoInputShaderProcessor::~XTwoInputShaderProcessor() {
    XImage::destroy(mTexture2);
}

void XTwoInputShaderProcessor::setSecondInputFrameBuffer(XFrameBuffer *secondFrame) {
    mSecondInputFrameBuffer = secondFrame;
}

void XTwoInputShaderProcessor::updateParams() {
    XShaderProcessor::updateParams();
    if (!bgfx::isValid(mTexture2)) {
        mTexture2 = bgfx::createUniform("s_texColor1", bgfx::UniformType::Sampler);
    }

    bgfx::setTexture(1, mTexture2, mSecondInputFrameBuffer->getTexture());
}

bool XTwoInputShaderProcessor::isValid() {
    if (mSecondInputFrameBuffer == nullptr || !bgfx::isValid(mSecondInputFrameBuffer->getTexture())) {
        LOGE("[XTwoInputShaderProcessor::submit] second input frame is invalid.");
        return false;
    }
    return XShaderProcessor::isValid();
}
NS_X_IMAGE_END