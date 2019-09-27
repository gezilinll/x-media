//
// Created by 林炳河 on 2019/9/21.
//

#ifndef GPUIMAGE_X_XFILTEREFFECT_HPP
#define GPUIMAGE_X_XFILTEREFFECT_HPP

#include "XEffect.hpp"
#include "filter/XFilter.hpp"
#include <unordered_map>
#include <glm/vec4.hpp>

NS_X_IMAGE_BEGIN
struct XFilterParam {
    std::string name;
    glm::vec4 valueMin;
    glm::vec4 valueMax;
    glm::vec4 valueDefault;
    glm::vec4 value;
    int valueNum;
};

/**
 * @brief 滤镜效果基类，负责控制滤镜通用逻辑
 *
 * 所有滤镜类都将具备以下几个内容：
 * 1.滤镜参数名
 * 2.滤镜参数的最大、最小以及默认值（特殊情况会在类注释上说明）
 * 3.滤镜参数的设置值
 * 通过以上内容来提供给使用者更直观的使用说明和参数设置
 */
class XFilterEffect: public XEffect {
public:
    XFilterEffect();

    virtual ~XFilterEffect();

    XInputOutput *get() override;

    XFilterParam getParam(std::string name);

    std::unordered_map<std::string, XFilterParam> getParams();

    void updateValue(std::string name, glm::vec4 value);

protected:
    /**
     * @brief 初始化接口，子类可继承该接口实现自身的初始化
     */
    virtual void init();

    void addParam(std::string name, float valueMin, float valueMax, float valueDefault, float value, int valueNum);

    void addParam(XFilterParam param);

    void setVertexShaderName(std::string name);

    void setFragmentShaderName(std::string name);

protected:
    XFilter *mFilter; /// 滤镜处理类
    std::string mVertexShaderName;
    std::string mFragmentShaderName;

private:
    std::unordered_map<std::string, XFilterParam> mParams;
};

/**
 * @brief 饱和度调节滤镜
 */
class XSaturation : public XFilterEffect {
public:
    XSaturation() : XFilterEffect() {
        setFragmentShaderName("fs_saturation");
        addParam("saturation", 0.0f, 2.0f, 1.0f, 1.0f, 1);
    }

    std::string getName() override {
        return "Saturation";
    }
};

/**
 * @brief 对比度调节滤镜
 */
class XContrast : public XFilterEffect {
public:
    XContrast() : XFilterEffect() {
        setFragmentShaderName("fs_contrast");
        addParam("contrast", 0.0f, 4.0f, 1.0f, 1.0f, 1);
    }

    std::string getName() override {
        return "Contrast";
    }
};

/**
 * @brief 亮度调节滤镜
 */
class XBrightness : public XFilterEffect {
public:
    XBrightness() : XFilterEffect() {
        setFragmentShaderName("fs_brightness");
        addParam("brightness", -1.0f, 1.0f, 0.0f, 0.0f, 1);
    }

    std::string getName() override {
        return "Brightness";
    }
};

/**
 * @brief HUE调节滤镜
 */
class XHUE : public XFilterEffect {
public:
    XHUE() : XFilterEffect() {
        setFragmentShaderName("fs_hue");
        addParam("hueAdjust", 0.0f, 2 * M_PI, 0.0f, 0.0f, 1);
    }

    std::string getName() override {
        return "HUE";
    }
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECT_HPP
