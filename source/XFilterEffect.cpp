//
// Created by 林炳河 on 2019/9/21.
//

#include "XFilterEffect.hpp"

NS_X_IMAGE_BEGIN
XFilterEffect::XFilterEffect() : XEffect() {
    mFilter = nullptr;
}

XFilterEffect::~XFilterEffect() {
    SAFE_DELETE(mFilter);
}

std::string XFilterEffect::getVertexShaderName() {
    return "vs_filter_normal";
}

std::string XFilterEffect::getFragmentShaderName() {
    return "fs_filter_normal";
}

XInputOutput* XFilterEffect::get() {
    if (mFilter == nullptr) {
        mFilter = new XFilter(getVertexShaderName(), getFragmentShaderName());
    }
    std::unordered_map<std::string, glm::vec4> values = getValues();
    for (std::pair<std::string, glm::vec4> value : values) {
        std::string &paramName = value.first;
        glm::vec4 &paramValue = value.second;
        float *valueArray = new float[4]{paramValue.x, paramValue.y, paramValue.z, paramValue.w};
        mFilter->setVec4(paramName, valueArray);
        SAFE_DELETE_ARRAY(valueArray);
    }
    return mFilter;
}

NS_X_IMAGE_END