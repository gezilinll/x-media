//
// Created by 林炳河 on 2019-09-25.
//

#include "XTwoInputFilter.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
XTwoInputFilter::XTwoInputFilter(const std::string vertex, const std::string fragment) : XFilter(vertex, fragment) {
    mSecondInputFrameBuffer = nullptr;
    mTexture2 = BGFX_INVALID_HANDLE;
}

void XTwoInputFilter::setSecondInputFrameBuffer(XFrameBuffer *secondFrame) {
    mSecondInputFrameBuffer = secondFrame;
}

void XTwoInputFilter::updateParams() {
    XFilter::updateParams();
    if (!bgfx::isValid(mTexture2)) {
        mTexture2 = bgfx::createUniform("s_texColor1", bgfx::UniformType::Sampler);
    }

    bgfx::setTexture(0, mTexture2, mSecondInputFrameBuffer->getTexture());
}

bool XTwoInputFilter::isValid() {
    if (mSecondInputFrameBuffer == nullptr || !bgfx::isValid(mSecondInputFrameBuffer->getTexture())) {
        LOGE("[XTwoInputFilter::submit] second input frame is invalid.");
        return false;
    }
    return XFilter::isValid();
}
NS_X_IMAGE_END