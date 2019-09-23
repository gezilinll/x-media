//
// Created by 林炳河 on 2019-09-23.
//

#ifndef GPUIMAGE_X_XEFFECTUI_HPP
#define GPUIMAGE_X_XEFFECTUI_HPP
#include "XEffect.hpp"

NS_X_IMAGE_BEGIN
class XEffectUI {
public:
    XEffectUI(XEffect *effect);

    void setIndex(int index);

    XEffect *getEffect();

    virtual void imgui();

    virtual void update();

protected:
    XEffect *mEffect;
    int mIndex;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XEFFECTUI_HPP
