//
// Created by 林炳河 on 2019/10/2.
//

#ifndef GPUIMAGE_X_XMIXER_HPP
#define GPUIMAGE_X_XMIXER_HPP

#include "XFilter.hpp"
#include "XTwoInputShaderProcessor.hpp"

NS_X_IMAGE_BEGIN
enum XMixerType {
    BLEND_NORMAL,
    BLEND_MULTIPLY,
    BLEND_ADD,

    MATTE_ALPHA,

    TRANSITION_FADE
};

class XMixer : public XFilter  {
public:
    XMixer(XMixerType type);

private:
    void init();

    bool initBlend();

    bool initMatte();

    bool initTransition();

private:
    XMixerType mType;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XMIXER_HPP
