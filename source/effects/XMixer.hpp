//
// Created by 林炳河 on 2019/10/2.
//

#ifndef GPUIMAGE_X_XMIXER_HPP
#define GPUIMAGE_X_XMIXER_HPP

#include "XFilter.hpp"
#include "XTwoInputEffectProcessor.hpp"

NS_X_IMAGE_BEGIN
enum XMixerType {
    /// 正常混合类型
    NORMAL,
    MULTIPLY,
    ADD,

    /// 抠图类型
    ALPHA,

    /// 转场类型
    FADE
};

class XMixer : public XFilter  {
public:
    XMixer(XMixerType type);

    bool isSame(XMixerType type);

private:
    void init();

private:
    XMixerType mType;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XMIXER_HPP
