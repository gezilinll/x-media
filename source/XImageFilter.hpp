//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "XImageOutput.hpp"
#include <string>

NS_X_IMAGE_BEGIN
class XImageFilter: public XImageInput, public XImageOutput {
public:
    XImageFilter(std::string vertex, std::string fragment);
    
    void newFrameReadyAtProgress(float progress, int index);

    void setInputFrameBuffer(XImageFrameBuffer* input);
    
    void setFloat(std::string paramName, float paramValue);

    void setVec3(std::string paramName, float *paramValue);

    void setVec4(std::string paramName, float *paramValue);
protected:
    std::string mVertexShaderPath;
    std::string mFragmentShaderPath;
    bgfx::ProgramHandle mProgram;
    bgfx::UniformHandle mUniformTexture;
    bgfx::VertexBufferHandle mVertexBuffer;
    bgfx::IndexBufferHandle mIndexBuffer;
    XImageFrameBuffer* mFirstInputFrameBuffer;

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
