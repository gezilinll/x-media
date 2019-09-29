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
     * @brief 销毁所有资源
     */
    static void destroy();

    /***** 安全释放bgfx中数据的接口列表 *****/
    static void destroy(bgfx::ProgramHandle &handle);
    static void destroy(bgfx::FrameBufferHandle &handle);
    static void destroy(bgfx::TextureHandle &handle);
    static void destroy(bgfx::VertexBufferHandle &handle);
    static void destroy(bgfx::IndexBufferHandle &handle);
    static void destroy(bgfx::UniformHandle &handle);

    static glm::vec4 wrapFloat(float value);

    static float* wrapFloatToVec4(float value);

    static float* wrapVec3ToVec4(float x, float y, float z);

    static float* wrapVec3ToVec4(float* xyz);

private:
    static int sRenderIndex; /// 渲染次序ID
    static XFrameBuffer *sFrame; /// 离屏渲染帧数据
    static std::vector<XLayer *> sLayers; /// 图层列表
    static std::vector<XEffect *> sGlobalEffects; /// 全局效果列表
    static bgfx::Init sInit;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEUTILS_HPP
