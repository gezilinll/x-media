//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "inout/XInputOutput.hpp"
#include <string>
#include <unordered_map>
#include <glm/vec4.hpp>

NS_X_IMAGE_BEGIN
/**
 * @brief 控制滤镜渲染逻辑的基类，包括支持简单的纹理渲染等功能
 *
 * 该类继承自 #XInputOutput ，因此支持输入帧数据并对输入帧数据处理，生成新的输出数据，进而形成链式效果
 * 该类通过设置不同的着色器以及按照不通用逻辑拆分类型（如输入纹理个数），使用时按照自己的类型来使用不同类型的通用对象并传入不同的着色器，
 * 参数设置上统一使用 #glm::vec4 来存放，如果是单float参数，那么会取向量的第一个参数，以此类推
 */
class XEffectProcessor: public XInputOutput {
public:
    XEffectProcessor(std::string vertex, std::string fragment);

    virtual ~XEffectProcessor();

    void submit() override;

    void setInputFrameBuffer(XFrameBuffer* input) override ;

    /**
     * @brief 设置变量数值
     * @param paramName 变量名
     * @param value 变量值
     */
    void setVec4(std::string paramName, glm::vec4 &value);

    /**
     * @brief 设置变换矩阵
     * @param view 相机变换矩阵
     * @param projection 投影矩阵
     */
    void setTransform(const float *view, const float *projection);

protected:
    void init() override ;

    /**
     * @brief 实际更新滤镜参数的接口，在 #submit 中调用避免多次调用同一个 uniform 的设置
     * @note bgfx在一轮渲染中如果对同一个uniform多次设置会崩溃
     * @note 子类可重载该接口并实现自身的参数设置逻辑，但是需要注意同步调用基类的该接口避免参数设置丢失
     */
    virtual void updateParams();

    /**
     * @brief 判断各项条件是否符合可渲染要求
     * @return 是否符合
     * @note 子类可重载该接口并结合基类该接口共同完成渲染要求的检验
     */
    virtual bool isValid();

protected:
    std::string mVertexShaderPath; /// 顶点着色器路径
    std::string mFragmentShaderPath; /// 片段着色器路径
    bgfx::ProgramHandle mProgram; /// 滤镜Program
    bgfx::UniformHandle mTexture; /// 一号纹理
    std::unordered_map<std::string, glm::vec4> mParams; /// 滤镜参数值容器
    std::unordered_map<std::string, bgfx::UniformHandle> mParamHandles; /// 滤镜参数uniform容器
    bgfx::VertexBufferHandle mVertexBuffer; /// 滤镜顶点缓冲
    bgfx::IndexBufferHandle mIndexBuffer; /// 滤镜顶点次序缓冲
    XFrameBuffer* mFirstInputFrameBuffer; /// 滤镜一号输入纹理
private:
    float *mViewMatrix; /// 相机变换矩阵
    float *mProjectionMatrix; /// 投影变换矩阵
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
