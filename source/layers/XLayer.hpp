//
// Created by 林炳河 on 2019/9/20.
//

#ifndef GPUIMAGE_X_XLAYER_HPP
#define GPUIMAGE_X_XLAYER_HPP

#include "XMacros.hpp"
#include "XModels.hpp"
#include "XEffect.hpp"
#include "XFrameBuffer.hpp"
#include "XBlend.hpp"
#include "bgfx/bgfx.h"
#include <vector>

NS_X_IMAGE_BEGIN
/**
 * @brief 图层基类，负责存储图层共用数据以及逻辑操作，该图层基类不可直接使用
 *
 * 每个图层都会有自身的一个初始输出，如帧图层即该帧渲染的结果，因此在渲染开始之前会通过调用内部接口 #updateSource 来进行初始数据的刷新，
 * 所有特定类型图层都需要继承自该基类并重载 #updateSource 接口实现自身的初始输出的更新（可自行控制是否需要重复更新），初始数据使用 #XImageOutput 来进行输出。
 * 每个图层都有自己的一个ID，该ID对应的就是 bgfx 中渲染的 View 的ID，因此ID不可重复，并且需要控制在 256 以内
 */
class XLayer {
public:
    /**
     * @brief 创建图层
     * @param id 图层ID
     * @attention 图层ID不可重复
     */
    XLayer(int id);

    virtual ~XLayer();

    /**
     * @brief 获取图层ID
     * @return 图层ID
     */
    int getID();

    /**
     * @brief 设置图层渲染区域
     * @param rect 渲染区域
     * @attention 宽高必须 > 0
     */
    void setViewRect(XRect &rect);

    /**
     * @brief 设置混合模式
     * @param blend 混合对象
     * @note 该图层的混合模式会作用于当前图层（在下）以及它的下一个图层（在上）
     */
    void setBlend(XBlend blend);

    /**
     * @brief 获取混合数据
     * @return 混合数据
     */
    XBlend getBlend();

    /**
     * @brief 获取图层渲染区域
     * @return 渲染区域
     */
    XRect getViewRect();

    /**
     * @brief 添加图层的叠加效果
     * @param effect 叠加效果
     * @attention 该对象的内存空间由外部控制释放
     */
    void addEffect(XEffect *effect);

    /**
     * @brief 设置效果链
     * @param effects 效果链
     * @attention 效果链的内存空间由外部控制释放
     */
    void setEffects(std::vector<XEffect *> effects);

    /**
     * @brief 获取图层所有的效果列表
     * @return 效果列表
     */
    std::vector<XEffect *> getEffects();

    /**
     * @brief 提交该图层数据进行渲染
     * @note
     * 1. 因为图层区域不一定是整个屏幕，因此在绘制时会将效果链的渲染区域设置成 {0,0,rect.width,rect.height} ，
     *    这一方面可以在非全屏情况下节省内存提升性能，另一方面避免了创建 rect 的宽高尺寸的FBO，但是设置给效果链绘制区域却不是从左上角
     *    [0,0] 开始，导致效果丢失等问题
     * 2. 在渲染的初始化步骤，除了设置区域外，还会将效果链都设置成渲染到帧数据中，包括最后一个效果
     */
    void submit();

    /**
     * @brief 获取图层叠加效果后的渲染结果
     * @return 渲染结果
     */
    XFrameBuffer* get();
protected:
    /**
     * @brief 更新图层初始输出内容
     * @attention 子类必须通过重该接口实现自身逻辑
     */
    virtual void updateSource();

private:
    XLayer();

protected:
    XOutput *mLayerSource; /// 存储图层初始内容，并作为效果链的开头
    XFrameBuffer* mLayerResult; /// 存储图层渲染结果的帧对象
    XRect mViewRect; /// 图层区域

private:
    int mID; /// 图层ID
    std::vector<XEffect *> mEffects; /// 叠加效果链
    XBlend mBlend; /// 混合数据
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XLAYER_HPP
