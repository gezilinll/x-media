//
// Created by 林炳河 on 2019/9/21.
//

#include "XFilter.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
XFilter::XFilter() : XEffect() {
    mFilter = nullptr;
    mVertexShaderName = "vs_default";
    mFragmentShaderName = "fs_default";
}

XFilter::XFilter(XFilterType type) : XFilter() {
    mType = type;
    init();
}

XFilter::~XFilter() {
    SAFE_DELETE(mFilter);
}

void XFilter::init() {
    if (mFilter == nullptr) {
        if (mType == XFilterType::SATURATION) {
            mFragmentShaderName = "fs_saturation";
            addParam("saturation", 0.0f, 2.0f, 1.0f, 1.0f);
            setName("Saturation");
        }else if (mType == XFilterType::CONTRAST) {
            mFragmentShaderName = "fs_contrast";
            addParam("contrast", 0.0f, 4.0f, 1.0f, 1.0f);
            setName("Contrast");
        } else if (mType == XFilterType::BRIGHTNESS) {
            mFragmentShaderName = "fs_brightness";
            addParam("brightness", -1.0f, 1.0f, 0.0f, 0.0f);
            setName("Brightness");
        } else if (mType == XFilterType::EXPOSURE) {
            mFragmentShaderName = "fs_exposure";
            addParam("exposure", -10.0f, 10.0f, 0.0f, 0.0f);
            setName("Exposure");
        } else if (mType == XFilterType::RGB) {
            mFragmentShaderName = "fs_rgb";
            addParam("redAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
            addParam("greenAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
            addParam("blueAdjustment", 0.0f, 10.0f, 1.0f, 1.0f);
            setName("RGB");
        } else if (mType == XFilterType::HUE) {
            mFragmentShaderName = "fs_hue";
            addParam("hueAdjust", 0.0f, 2 * M_PI, 0.0f, 0.0f);
            setName("HUE");
        } else if (mType == XFilterType::LEVELS) {
            mFragmentShaderName = "fs_levels";
            glm::vec4 valueZero = {0.0f, 0.0f, 0.0f, 0.0f};
            glm::vec4 valueOne = {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 valueHundred = {100.0f, 100.0f, 100.0f, 100.0f};
            addParam("levelMinimum", valueZero, valueOne, valueZero, valueZero, 3);
            addParam("levelMaximum", valueZero, valueOne, valueOne, valueOne, 3);
            addParam("levelMiddle", valueZero, valueHundred, valueOne, valueOne, 3);
            addParam("minOutput", valueZero, valueOne, valueZero, valueZero, 3);
            addParam("maxOutput", valueZero, valueOne, valueOne, valueOne, 3);
            setName("Levels");
        } else if (mType == XFilterType::WHITE_BALANCE) {
            mFragmentShaderName = "fs_white_balance";
            addParam("temperature", -50.0f, 50.0f, 0.0f, 0.0f);
            addParam("tint", -10.0f, 10.0f, 0.0f, 0.0f);
            setName("WhiteBalance");
        } else if (mType == XFilterType::MONOCHROME) {
            mFragmentShaderName = "fs_monochrome";
            addParam("intensity", 0.0f, 1.0f, 0.0f, 0.0f);
            glm::vec4 valueMin = {0.0f, 0.0f, 0.0f, 0.0f};
            glm::vec4 valueMax = {1.0f, 1.0f, 1.0f, 1.0f};
            glm::vec4 valueDefault = {0.5f, 0.5f, 0.5f, 0.5f};
            glm::vec4 value = valueDefault;
            addParam("filterColor", valueMin, valueMax, valueDefault, value, 3);
            setName("Monochrome");
        }
        mFilter = new XEffectProcessor(mVertexShaderName, mFragmentShaderName);
    }

}

XInputOutput* XFilter::get() {
    for (std::pair<std::string, XFilterParam> value : mParams) {
        std::string &paramName = value.first;
        XFilterParam &param = value.second;
        mFilter->setVec4(paramName, param.value);
    }
    return mFilter;
}

void XFilter::updateValue(std::string name, glm::vec4 value) {
    mParams[name].value = value;
}

void XFilter::updateValue(std::unordered_map<std::string, glm::vec4> params) {
    for (auto iter : params) {
        mParams[iter.first].value = iter.second;
    }
}

XFilterParam XFilter::getParam(std::string name) {
    return mParams.find(name)->second;
}

std::unordered_map<std::string, XFilterParam> XFilter::getParams() {
    return mParams;
}

void XFilter::addParam(XFilterParam param) {
    mParams[param.name] = param;
}

void XFilter::addParam(std::string name,float valueMin, float valueMax, float valueDefault, float value) {
    XFilterParam param = {name, XImage::wrapFloat(valueMin), XImage::wrapFloat(valueMax),
                          XImage::wrapFloat(valueDefault), XImage::wrapFloat(value), 1};
    mParams[name] = param;
}

void XFilter::addParam(std::string name, glm::vec4 valueMin, glm::vec4 valueMax, glm::vec4 valueDefault,
                             glm::vec4 value, int valueNum) {
    XFilterParam param = {name, valueMin, valueMax, valueDefault, value, valueNum};
    mParams[name] = param;
}

NS_X_IMAGE_END