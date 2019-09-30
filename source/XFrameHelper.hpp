//
// Created by 林炳河 on 2019-09-26.
//

#ifndef GPUIMAGE_X_XFRAMEHELPER_HPP
#define GPUIMAGE_X_XFRAMEHELPER_HPP

#include "XLayer.hpp"
#include <XModels.hpp>

NS_X_IMAGE_BEGIN
/**
 * @brief 负责图层渲染后将图层混合、叠加全局效果等工作的工具类
 */
class XFrameHelper {
public:
    /**
     * @brief 将图层渲染结果混合并叠加全局特效
     * @param layers 图层列表
     * @param effects 全局特效列表
     * @param toResult 保存最终渲染结果的帧对象
     */
    static void blendAndEffects(std::vector<XLayer *> layers, std::vector<XEffect *> effects, XFrameBuffer* toResult);

private:
    /**
     * @brief 将图层渲染结果进行混合
     * @param layers 图层列表
     * @param toResult 保存混合结果的帧对象
     */
    static void blend(std::vector<XLayer *> layers, XFrameBuffer *toResult);

    /**
     * @brief 将图层混合结果进行全局特效叠加
     * @param layers 全局特效列表
     * @param toResult 保存叠加效果后结果的帧对象
     */
    static void effect(XFrameBuffer *blendResult, std::vector<XEffect *> effects, XFrameBuffer* toResult);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFRAMEHELPER_HPP
