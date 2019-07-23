//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "XImageOutput.hpp"
#include <string>

NS_X_IMAGE_BEGIN
/**
 * XImage's base filter class and control all of effects unless they have params isn't common, which means we switch effect by shader path not filter class.
 * Like one pass, two pass, three pass, ..., group, they all have their own action, params and logic.
 * Filters and other subsequent elements in the chain inherit to the XImageInput class,
 * which lets them take in the supplied or processed texture from the previous link in the chain and do something with it.
 * Objects one step further down the chain are considered targets, and processing can be branched by adding multiple targets to a single output or filter.
 */
class XImageFilter: public XImageInput, public XImageOutput {
public:
    /**
     * XImage's filter constructor
     * @param vertex vertex shader path
     * @param fragment fragment shader path
     */
    XImageFilter(std::string vertex, std::string fragment);

    /**
     * Notify new frame is ready and can process it
     * @param progress current progress, [0.0, 1.0]
     * @param index index of this filter in the render sequence
     */
    void newFrameReadyAtProgress(float progress, int index);

    /**
     * Set frame buffer to be processing
     * @param input frame buffer
     */
    void setInputFrameBuffer(XImageFrameBuffer* input);

    /**
     * Set param value of shader, this param must be Vec4 type Because of bgfx only support vec4 type as uniform param
     * The filter's param such as float/vec2... can pass value like this: {floatValue, 1.0f, 1.0f, 1.0f}/{x, y, 1.0f, 1.0f}
     * Perhaps you would like to use {@link XImageUtils} to wrap your value easily
     * @param paramName name of param
     * @param paramValue value of param
     */
    void setVec4(std::string paramName, float *paramValue);
protected:
    std::string mVertexShaderPath; /// vertex shader path
    std::string mFragmentShaderPath; /// fragment shader path
    bgfx::ProgramHandle mProgram; /// filter program handle
    bgfx::UniformHandle mUniformTexture; /// filter texture param handle
    bgfx::VertexBufferHandle mVertexBuffer; /// filter vertex buffer handle
    bgfx::IndexBufferHandle mIndexBuffer; /// filter index buffer handle
    XImageFrameBuffer* mFirstInputFrameBuffer; /// filter input frame buffer which is used to be texture
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
