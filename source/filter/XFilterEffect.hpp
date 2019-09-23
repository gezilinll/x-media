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

protected:
    /**
     * @brief 获取滤镜顶点着色器文件名
     * @return 顶点着色器文件名
     */
    virtual std::string getVertexShaderName();

    /**
     * @brief 获取滤镜片段着色器文件名
     * @return 片段着色器文件名
     */
    virtual std::string getFragmentShaderName();

    /**
     * @brief 获取滤镜的参数名与参数值列表
     * @return 参数列表
     */
    virtual std::unordered_map<std::string, glm::vec4> getValues() = 0;

private:
    XFilter *mFilter; /// 滤镜处理类
};

/**
 * @brief 饱和度调节滤镜
 */
class XSaturation : public XFilterEffect {
public:
    std::string paramSaturation = "saturation";
    float paramSaturationMin = 0.0f;
    float paramSaturationMax = 2.0f;
    float paramSaturationDefault = 1.0f;
    float paramSaturationValue = paramSaturationDefault;

protected:
    std::string getFragmentShaderName() override {
        return "fs_filter_saturation";
    }

    std::unordered_map<std::string, glm::vec4> getValues() override {
        std::unordered_map<std::string, glm::vec4> result;
        result.insert(std::make_pair(paramSaturation, paramSaturationValue));
        return result;
    }
};

/**
 * @brief 对比度调节滤镜
 */
class XContrast : public XFilterEffect {
public:
    std::string paramContrast = "contrast";
    float paramContrastMin = 0.0f;
    float paramContrastMax = 4.0f;
    float paramContrastDefault = 1.0f;
    float paramContrastValue = paramContrastDefault;

protected:
    std::string getFragmentShaderName() override {
        return "fs_filter_contrast";
    }

    std::unordered_map<std::string, glm::vec4> getValues() override {
        std::unordered_map<std::string, glm::vec4> result;
        result.insert(std::make_pair(paramContrast, paramContrastValue));
        return result;
    }
};

/**
 * @brief 亮度调节滤镜
 */
class XBrightness : public XFilterEffect {
public:
    std::string paramBrightness = "brightness";
    float paramBrightnessMin = -1.0f;
    float paramBrightnessMax = 1.0f;
    float paramBrightnessDefault = 0.0f;
    float paramBrightnessValue = paramBrightnessDefault;

protected:
    std::string getFragmentShaderName() override {
        return "fs_filter_brightness";
    }

    std::unordered_map<std::string, glm::vec4> getValues() override {
        std::unordered_map<std::string, glm::vec4> result;
        result.insert(std::make_pair(paramBrightness, paramBrightnessValue));
        return result;
    }
};

/**
 * @brief HUE调节滤镜
 */
class XHUE : public XFilterEffect {
public:
    std::string paramHUE = "hueAdjust";
    float paramHUEMin = 0.0f;
    float paramHUEMax =  2 * M_PI;
    float paramHUEDefault = 0.0f;
    float paramHUEValue = paramHUEDefault;

protected:
    std::string getFragmentShaderName() override {
        return "fs_filter_hue";
    }

    std::unordered_map<std::string, glm::vec4> getValues() override {
        std::unordered_map<std::string, glm::vec4> result;
        result.insert(std::make_pair(paramHUE, paramHUEValue));
        return result;
    }
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECT_HPP
