//
// Created by 林炳河 on 2019/9/20.
//

#ifndef GPUIMAGE_X_XLAYER_HPP
#define GPUIMAGE_X_XLAYER_HPP

#include "XMacros.hpp"
#include "XModels.hpp"
#include "effects/XEffect.hpp"
#include "XFrameBuffer.hpp"
#include "XMixer.hpp"
#include "bgfx/bgfx.h"
#include <vector>

NS_X_IMAGE_BEGIN
/**
 * @brief 图层基类，负责存储图层共用数据以及逻辑操作，该图层基类不可直接使用
 *
 * 每个图层都会有自身的一个初始输出，如帧图层即该帧渲染的结果，因此在渲染开始之前会通过调用内部接口 #updateSource 来进行初始数据的刷新，
 * 所有特定类型图层都需要继承自该基类并重载 #updateSource 接口实现自身的初始输出的更新（可自行控制是否需要重复更新），初始数据使用 #XImageOutput 来进行输出。
 * 每个图层都有自己的一个ID，并且ID不可重复，该ID与渲染顺序无关
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
     * @brief 设置图层源数据输出对象
     * @param source 源数据输出对象
     * @attention 该对象生命周期由外部管理
     */
    void setSource(XOutput *source);

    /**
     * @brief 设置图层渲染区域
     * @param rect 渲染区域
     * @attention 宽高必须 > 0
     */
    void setViewRect(XRect &rect);

    /**
     * @brief 设置混合器
     * @param mixture 混合类型
     * @note 该图层的混合器会作用于当前图层（在下）以及它的下一个图层（在上）
     * @attention 该混合器的内存管理由XLayer内部负责
     */
    void setMixer(XMixerType type);

    /**
     * @brief 更新混合器参数值
     * @param name 参数名
     * @param value 更新值
     * @note 在更新参数前确保调用 #setMixer 设置过对应的混合器类型
     */
    void updateMixerValue(std::string name, glm::vec4 value);

    /**
    * @brief 获取混合器
    * @return 混合器
    */
    XMixer* getMixer();

    /**
     * @brief 添加图层内遮罩
     * @param mask 遮罩层
     * @note 混合效果规则为，使用当前要混合的遮罩层的遮罩器将遮罩层（在上）被混合的帧（当前图层或上一次遮罩混合结果，在下）进行混合
     * @attention 用作图层内遮罩的图层将只用作遮罩混合，不再与其他图层叠加
     * @attention 用于图层内遮罩的图层暂不支持进行叠加特效，在渲染时会将其特效列表进行清空
     * @attention 用于图层内遮罩的图层暂不支持再该遮罩图层上添加遮罩图层，在渲染时会将其遮罩图层列表进行清空
     */
    void addMask(XLayer *mask);

    /**
     * @brief 清空图层内遮罩列表
     */
    void clearMasks();

    /**
     * @brief 设置图层间遮罩
     * @param matte 遮罩图层
     * @todo 该功能具体逻辑待补充
     */
    void setMatte(XLayer *matte);

    /**
     * @brief 设置该图层是否可见
     * @param isVisible 是否可见
     */
    void setVisibility(bool isVisible);

    /**
     * @brief 设置渲染层级
     * @param zOrder 渲染层级
     * @note 在渲染层级相同时按照图层插入顺序为准，否则层级越高的图层在越上面
     */
    void setZOrder(int zOrder);

    /**
     * @brief 获取图层渲染层级
     * @return 渲染层级
     */
    int getZOrder();

    /**
     * @brief 该图层是否可见
     * @return 是否可见
     */
    bool isVisible();

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
     * @brief 清空特效列表
     */
    void clearEffects();

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

private:
    XLayer();

protected:
    XOutput *mLayerSource; /// 存储图层初始内容，并作为效果链的开头
    XFrameBuffer* mLayerResult; /// 存储图层渲染结果的帧对象
    XRect mViewRect; /// 图层区域

private:
    int mID; /// 图层ID
    std::vector<XEffect *> mEffects; /// 叠加效果链
    XMixer *mMixer; /// 混合器
    std::vector<XLayer *> mMattes; /// 图层件遮罩列表
    bool mIsVisible; /// 图层是否可见
    int mZOrder; /// 图层渲染层级
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XLAYER_HPP
