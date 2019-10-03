//
// Created by 林炳河 on 2019/7/21.
//

#ifndef GPUIMAGE_X_XIMAGEUTILS_HPP
#define GPUIMAGE_X_XIMAGEUTILS_HPP

#include "bgfx/bgfx.h"
#include "bgfx_utils.h"
#include "XLayer.hpp"
#include "glm/vec4.hpp"
#include <string>

NS_X_IMAGE_BEGIN

/**
 * @brief XImage's utils to make develop easier.
 */
/**
 * @brief 负责对外提供统一接口、逻辑控制、部分工具能力等内容的门面类，使用者应通过该类来实现主要的渲染数据设置和逻辑控制等
 *
 * 主要使用流程如下：
 * 1. #init 初始化必要参数
 * 3. #begin 开始新一轮渲染流程
 * 4. 添加/更新图层等数据
 * 5. #submit 提交数据进行离屏渲染
 * 6. #frame 将离屏渲染结果渲染至屏幕
 * 7. #end 结束当前轮渲染
 * 8. #shutdown 关闭渲染引擎，只在不再进行渲染时调用
 */
class XImage {
public:
    static void init(bgfx::Init &init);

    /**
     * @brief 开始进入渲染流程
     */
    static void begin();

    /**
     * @brief 添加图层，图层数据直接在图层对象上设置
     * @param layer 图层
     * @attention 该图层对象的内存管理由创建者负责，该类不进行控制
     */
    static void addLayer(XLayer *layer);

    /**
     * @brief 添加全局性的效果，该效果会叠加在所有图层之上
     * @param globalEffect 效果
     */
    static void addGlobalEffect(XEffect *globalEffect);

    /**
     * @brief 获取画布宽度
     * @return 画布宽度
     * @attention 需要先调用 #init
     */
    static int getCanvasWidth();

    /**
     * @brief 获取画布高度
     * @return 画布高度
     * @attention 需要先调用 #init
     */
    static int getCanvasHeight();

    /**
     * @brief 提交包含图层等在内的渲染数据并进行离屏渲染
     * @note 该接口不会将渲染结果直接呈现在屏幕上
     */
    static void submit();

    /**
     * @brief 将调用 #submit 后的离屏渲染的结果渲染到屏幕上
     */
    static void frame();

    /**
     * @brief 获取下一次渲染的视图ID，在各个渲染环节时需要调用该接口来获取自身的渲染次序
     * @return 下一次渲染次序的ID
     * @attention 该渲染ID会在调用 #end 时进行重置为-1
     */
    static int nextRenderIndex();

    /**
     * @brief 获取当前渲染到的视图的ID
     * @return 当前渲染次序的ID
     * @attention 该渲染ID会在调用 #end 时进行重置为-1
     */
    static int currentRenderIndex();

    /**
     * @brief 结束该轮渲染流程
     */
    static void end();

    /**
     * @brief 关闭渲染引擎并销毁所有资源
     * @warning 该接口必须在使用方将图层等外部创建、管理的资源释放后调用，并需要确保在最后一帧渲染结束后
     */
    static void shutdown();

    /***** 安全释放bgfx中数据的接口列表 *****/
    static void destroy(bgfx::ProgramHandle &handle);
    static void destroy(bgfx::FrameBufferHandle &handle);
    static void destroy(bgfx::TextureHandle &handle);
    static void destroy(bgfx::VertexBufferHandle &handle);
    static void destroy(bgfx::IndexBufferHandle &handle);
    static void destroy(bgfx::UniformHandle &handle);

    /**
     * @brief 将float参数转为四位向量
     * @param value 参数
     * @return 向量
     */
    static glm::vec4 wrapFloat(float value);

private:
    /**
     * @brief 过滤并将符合过滤条件的图层进行排序，返回最终的图层列表
     * @return 符合渲染条件的排序后的图层列表
     * @note 图层必须可见并且非图层内遮罩以及作为图层间遮罩的同时需要参与最后全局图层混合的遮罩图层
     */
    static std::vector<XLayer *> filterSortLayers();

    /**
     * @brief 按照图层的渲染层级进行排序
     * @param a 图层a
     * @param b 图层b
     * @return a图层是否必b图层层级低
     */
    static bool layerZOrderCompare(XLayer *a, XLayer *b);

private:
    static int sRenderIndex; /// 渲染次序ID
    static XFrameBuffer *sFrame; /// 离屏渲染帧数据
    static std::vector<XLayer *> sLayers; /// 图层列表
    static std::vector<XEffect *> sGlobalEffects; /// 全局效果列表
    static bgfx::Init sInit; /// 初始化数据
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEUTILS_HPP
