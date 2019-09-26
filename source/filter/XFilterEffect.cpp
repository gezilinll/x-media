//
// Created by 林炳河 on 2019/9/21.
//

#include "XFilterEffect.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XFilterEffect::XFilterEffect() : XEffect() {
    mFilter = nullptr;
    mVertexShaderName = "vs_filter_normal";
    mFragmentShaderName = "fs_filter_normal";
}

XFilterEffect::~XFilterEffect() {
    SAFE_DELETE(mFilter);
}

void XFilterEffect::init() {
    if (mFilter == nullptr) {
        mFilter = new XFilter(mVertexShaderName, mFragmentShaderName);
    }
}

XInputOutput* XFilterEffect::get() {
    init();
    for (std::pair<std::string, XFilterParam> value : mParams) {
        std::string &paramName = value.first;
        XFilterParam &param = value.second;
        mFilter->setVec4(paramName, param.value);
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

std::unordered_map<std::string, XFilterParam> XFilterEffect::getParams() {
    return mParams;
}

void XFilterEffect::addParam(XImageNS::XFilterParam param) {
    mParams[param.name] = param;
}

void XFilterEffect::addParam(std::string name,float valueMin, float valueMax,
        float valueDefault, float value, int valueNum) {
    XFilterParam param = {name, XImage::wrapFloat(valueMin), XImage::wrapFloat(valueMax),
                          XImage::wrapFloat(valueDefault), XImage::wrapFloat(value), valueNum};
    mParams[name] = param;
}

NS_X_IMAGE_END