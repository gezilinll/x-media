//
// Created by 林炳河 on 2019/9/21.
//

#ifndef GPUIMAGE_X_XFILTEREFFECT_HPP
#define GPUIMAGE_X_XFILTEREFFECT_HPP

#include "XEffect.hpp"
#include "XEffectProcessor.hpp"
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

enum XFilterType {
    NORMAL,
    SATURATION,
    CONTRAST,
    BRIGHTNESS,
    HUE,
    EXPOSURE,
    RGB,
    WHITE_BALANCE,
    LEVELS,
    MONOCHROME
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
class XFilter: public XEffect {
public:
    XFilter(XFilterType type);

    virtual ~XFilter();

    XInputOutput *get() override;

    /**
     * @brief 获取指定的参数数据
     * @param name 参数名
     * @return 参数数据
     */
    XFilterParam getParam(std::string name);

    /**
     * @brief 更新指定参数值
     * @param name 参数名
     * @param value 参数值
     */
    void updateValue(std::string name, glm::vec4 value);

    /**
     * @brief 更新参数值
     * @param params 参数列表
     */
    void updateValue(std::unordered_map<std::string, glm::vec4> params);

    /**
     * @brief 获取参数集合
     * @return 参数集合
     */
    std::unordered_map<std::string, XFilterParam> getParams();

protected:
    XFilter();

    /**
     * @brief 添加参数信息到参数列表中
     * @param name 参数名
     * @param valueMin 最小值
     * @param valueMax 最大值
     * @param valueDefault 默认值
     * @param value 当前值
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(std::string name, float valueMin, float valueMax, float valueDefault, float value);

    /**
     * @brief 添加参数信息到参数列表中
     * @param name
     * @param valueMin 最小值
     * @param valueMax 最大值
     * @param valueDefault 默认值
     * @param value 当前值
     * @param valueNum 四位向量中有效值的位数
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(std::string name, glm::vec4 valueMin, glm::vec4 valueMax, glm::vec4 valueDefault, glm::vec4 value, int valueNum);

    /**
     * @brief 添加参数信息到参数列表中
     * @param param 参数信息
     * @attention 参数如果已存在那么会直接替换掉已有参数
     */
    void addParam(XFilterParam param);

private:
    /**
     * @brief 初始化接口，初始化各项参数和滤镜
     */
    void init();

protected:
    XEffectProcessor *mFilter; /// 滤镜处理类
    std::string mVertexShaderName; /// 顶点着色器名
    std::string mFragmentShaderName; /// 片段着色器名

private:
    std::unordered_map<std::string, XFilterParam> mParams; /// 参数集合
    XFilterType mType;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFILTEREFFECT_HPP
