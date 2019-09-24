//
// Created by 林炳河 on 2019-09-23.
//

#ifndef GPUIMAGE_X_XFILTEREFFECTUI_HPP
#define GPUIMAGE_X_XFILTEREFFECTUI_HPP

#include "XEffectUI.hpp"
#include "XFilterEffect.hpp"

NS_X_IMAGE_BEGIN
class XFilterEffectUI : public XEffectUI {
public:
    XFilterEffectUI(XFilterEffect *effect);

    void imgui() override ;

    void update() override ;

private:
    std::string getLocationName(int i);

private:
    std::unordered_map<std::string, XFilterParam> mParams;
};

NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECTUI_HPP
