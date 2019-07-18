//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILTER_HPP
#define GPUIMAGE_X_XIMAGEFILTER_HPP

#include "XImageOutput.hpp"
#include <string>

class XImageFilter: public XImageInput, public XImageOutput {
public:
    XImageFilter();
    
    void newFrameReadyAtProgress(float progress, int index);

    void setInputFrameBuffer(XImageFrameBuffer* input);
protected:
    bgfx::ProgramHandle mProgram;
    bgfx::UniformHandle mUniformTexture;
    bgfx::VertexBufferHandle mVertexBuffer;
    bgfx::IndexBufferHandle mIndexBuffer;
    XImageFrameBuffer* mFirstInputFrameBuffer;

};


#endif //GPUIMAGE_X_XIMAGEFILTER_HPP
