//
// Created by 林炳河 on 2019/9/20.
//

#ifndef GPUIMAGE_X_XEFFECT_HPP
#define GPUIMAGE_X_XEFFECT_HPP

#include "inout/XInputOutput.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 效果数据与逻辑控制功能基类，用于提供各种类型效果的处理能力
 *
 * 该类直接使用无任何效果
 */
class XEffect {
public:
    XEffect();

    virtual ~XEffect();

    /**
     * @brief 获取可处理该效果的对象，可作为效果链中的一个环节
     * @return 效果处理对象
     * @attention 继承自该效果的子类需要重载该接口实现自身效果
     */
    virtual XInputOutput *get();

    /**
     * @brief 获取效果名
     * @return 效果名
     */
    virtual std::string getName();

protected:
    void setName(std::string name);

private:
    std::string mEffectName;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XEFFECT_HPP
