//
// Created by 林炳河 on 2019/9/21.
//

#include "XFilterEffect.hpp"

NS_X_IMAGE_BEGIN
XFilterEffect::XFilterEffect() : XEffect() {
    mFilter = nullptr;
    mVertexShaderName = "vs_filter_normal";
    mFragmentShaderName = "fs_filter_normal";
}

XFilterEffect::~XFilterEffect() {
    SAFE_DELETE(mFilter);
}

XInputOutput* XFilterEffect::get() {
    if (mFilter == nullptr) {
        mFilter = new XFilter(mVertexShaderName, mFragmentShaderName);
    }
    for (std::pair<std::string, XFilterParam> value : mParams) {
        std::string &paramName = value.first;
        XFilterParam &param = value.second;
        float *valueArray = new float[4]{param.value.x, param.value.y, param.value.z, param.value.w};
        mFilter->setVec4(paramName, valueArray);
    }
    return mFilter;
}

void XFilterEffect::setVertexShaderName(std::string name) {
    mVertexShaderName = name;
}

void XFilterEffect::setFragmentShaderName(std::string name) {
    mFragmentShaderName = name;
}

void XFilterEffect::updateValue(std::string name, glm::vec4 value) {
    mParams[name].value = value;
}

XFilterParam XFilterEffect::getParam(std::string name) {
    return mParams.find(name)->second;
}

void XFilterEffect::addParam(XImageNS::XFilterParam param) {
    mParams[param.name] = param;
}

void XFilterEffect::addParam(std::string name,float valueMin, float valueMax,
        float valueDefault, float value, int valueNum) {
    glm::vec4 vecMin = {valueMin, valueMin, valueMin, valueMin};
    glm::vec4 vecMax = {valueMax, valueMax, valueMax, valueMax};
    glm::vec4 vecDefault = {valueDefault, valueDefault, valueDefault, valueDefault};
    glm::vec4 vecValue = {value, value, value, value};
    XFilterParam param = {name, vecMin, vecMax, vecDefault, vecValue, valueNum};
    mParams[name] = param;
}

NS_X_IMAGE_END