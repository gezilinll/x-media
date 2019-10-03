//
// Created by 林炳河 on 2019/10/2.
//

#ifndef GPUIMAGE_X_XMIXER_HPP
#define GPUIMAGE_X_XMIXER_HPP

#include "XFilterEffect.hpp"
#include "XTwoInputFilter.hpp"

NS_X_IMAGE_BEGIN
enum XMixture {
    BLEND_NORMAL,
    BLEND_MULTIPLY,
    BLEND_ADD,

    MATTE_ALPHA,

    TRANSITION_FADE
};

class XMixer : public XFilterEffect  {
public:
    XMixer(XMixture mixture);

protected:
    void init() override;

private:
    bool setBlendFragment();

    bool setMatteFragment();

    bool setTransitionFragment();

private:
    XMixture mMixture;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XMIXER_HPP
