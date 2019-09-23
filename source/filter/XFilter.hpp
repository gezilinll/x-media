//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "inout/XInputOutput.hpp"
#include <string>
#include <unordered_map>

NS_X_IMAGE_BEGIN
/**
 * @brief XImage's base filter class and control all of effects unless they have params isn't common, which means we switch effect by shader path not filter class.
 * Like one pass, two pass, three pass, ..., group, they all have their own action, params and logic.
 * Filters and other subsequent elements in the chain inherit to the XImageInput class,
 * which lets them take in the supplied or processed texture from the previous link in the chain and do something with it.
 * Objects one step further down the chain are considered targets, and processing can be branched by adding multiple targets to a single output or filter.
 *
 * As to shader you can see \link XImageShaderInfo.hpp XImageShaderInfo \endlink for more information.
 */
class XFilter: public XInputOutput {
public:
    XFilter(std::string vertex, std::string fragment);

    ~XFilter();

    void submit() override;

    void setInputFrameBuffer(XFrameBuffer* input) override ;

    void setVec4(std::string paramName, float *paramValue);

private:
    void updateParams();

protected:
    void init() override ;

private:
    /**
     * @brief check if the view rect args is valid
     * @retval true YES
     * @retval false NO
     */
    bool isViewRectValid();

protected:
    std::string mVertexShaderPath; /// vertex shader path
    std::string mFragmentShaderPath; /// fragment shader path
    bgfx::ProgramHandle mProgram; /// filter program handle
    std::unordered_map<std::string, float*> mParams;
    std::unordered_map<std::string, bgfx::UniformHandle> mParamHandles; /// filter params of uniform type
    bgfx::VertexBufferHandle mVertexBuffer; /// filter vertex buffer handle
    bgfx::IndexBufferHandle mIndexBuffer; /// filter index buffer handle
    XFrameBuffer* mFirstInputFrameBuffer; /// filter input frame buffer which is used to be texture
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
