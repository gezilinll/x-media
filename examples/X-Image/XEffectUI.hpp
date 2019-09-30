//
// Created by 林炳河 on 2019-09-23.
//

#ifndef GPUIMAGE_X_XEFFECTUI_HPP
#define GPUIMAGE_X_XEFFECTUI_HPP
#include "effects/XEffect.hpp"

NS_X_IMAGE_BEGIN
class XEffectUI {
public:
    XEffectUI(XEffect *effect);

    virtual ~XEffectUI();

    XEffect *getEffect();

    virtual void imgui();

    virtual void update();

protected:
    XEffect *mEffect;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XEFFECTUI_HPP
