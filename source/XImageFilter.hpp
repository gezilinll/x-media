//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "XImageInputOutput.hpp"
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
class XImageFilter: public XImageInputOutput {
public:
    /**
     * @brief XImage's filter constructor
     * @param[in] vertex vertex shader path
     * @param[in] fragment fragment shader path
     */
    XImageFilter(std::string vertex, std::string fragment);

    ~XImageFilter();

    void renderAtProgress(float progress) override;

    void setInputFrameBuffer(XImageFrameBuffer* input) override ;

    /**
     * @brief set view rect to show render result
     * @param[in] x start x position of rect
     * @param[in] y start y position of rect
     * @param[in] width width of rect
     * @param[in] height height of rect
     */
    void setViewRect(int x, int y, int width, int height);

    /**
     * @brief set param value of shader, this param must be Vec4 type because of bgfx only support vec4 type as uniform param
     * The filter's param such as float/vec2... can pass value like this: {floatValue, 1.0f, 1.0f, 1.0f}/{x, y, 1.0f, 1.0f}
     * Perhaps you would like to use \link XImageUtils \endlink to wrap your value easily
     * @param[in] paramName name of param
     * @param[in] paramValue value of param
     */
    void setVec4(std::string paramName, float *paramValue);

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
    std::unordered_map<std::string, bgfx::UniformHandle> mUniformHandles; /// filter params of uniform type
    bgfx::VertexBufferHandle mVertexBuffer; /// filter vertex buffer handle
    bgfx::IndexBufferHandle mIndexBuffer; /// filter index buffer handle
    XImageFrameBuffer* mFirstInputFrameBuffer; /// filter input frame buffer which is used to be texture

    int mViewRectX, mViewRectY, mViewRectWidth, mViewRectHeight; /// rect to show result
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
