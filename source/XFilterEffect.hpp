//
// Created by 林炳河 on 2019/9/21.
//

#ifndef GPUIMAGE_X_XFILTEREFFECT_HPP
#define GPUIMAGE_X_XFILTEREFFECT_HPP

#include "XEffect.hpp"
#include "XFilter.hpp"
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
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECT_HPP
