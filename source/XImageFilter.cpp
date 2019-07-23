//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFilter.hpp"
#include "XImageUtils.hpp"

NS_X_IMAGE_BEGIN
//顶点以及纹理坐标对象
struct PosTexVertex
{
    float m_x;
    float m_y;
    float m_z;
    float t_x;
    float t_y;

    static void init()
    {
        ms_decl
                .begin()
                .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
                .add(bgfx::Attrib::TexCoord0,   2, bgfx::AttribType::Float)
                .end();
    };

    static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl PosTexVertex::ms_decl;
//顶点坐标数据
static PosTexVertex s_vertices[] =
        {
                {-1.0f,  1.0f,  0.0f, 0.0f, 0.0f },
                { 1.0f,  1.0f,  0.0f, 1.0f, 0.0f },
                {-1.0f, -1.0f,  0.0f, 0.0f, 1.0f },
                { 1.0f, -1.0f,  0.0f, 1.0f, 1.0f },
        };
//顶点顺序
static const uint16_t s_triList[] =
        {
                0, 1, 2, // 0
                1, 3, 2,
        };

XImageFilter::XImageFilter(std::string vertex, std::string fragment) {
    mVertexShaderPath = vertex;
    mFragmentShaderPath = fragment;
    mProgram = BGFX_INVALID_HANDLE;
    mUniformTexture = BGFX_INVALID_HANDLE;
    mVertexBuffer = BGFX_INVALID_HANDLE;
    mIndexBuffer = BGFX_INVALID_HANDLE;
}

void XImageFilter::setInputFrameBuffer(XImageFrameBuffer *input) {
    mFirstInputFrameBuffer = input;
}

void XImageFilter::newFrameReadyAtProgress(float progress, int index) {
    if (!bgfx::isValid(mProgram)) {
        mProgram = XImageUtils::loadProgram(mVertexShaderPath.data(), mFragmentShaderPath.data());
        mUniformTexture = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        // Create vertex stream declaration.
        PosTexVertex::init();

        // Create static vertex buffer.
        mVertexBuffer = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_vertices, sizeof(s_vertices) )
                , PosTexVertex::ms_decl
        );

        // Create static index buffer for triangle list rendering.
        mIndexBuffer = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_triList, sizeof(s_triList) )
        );
    }

    uint64_t state = 0
                     | BGFX_STATE_WRITE_R
                     | BGFX_STATE_WRITE_G
                     | BGFX_STATE_WRITE_B
                     | BGFX_STATE_WRITE_A
                     | UINT64_C(0)
    ;
    bgfx::setViewRect(0, 0, 0, 960, 720);//设置View0的视口
    bgfx::setVertexBuffer(0, mVertexBuffer);//设置stream0的vertexBuffer，注意第一个参数不是viewId
    bgfx::setIndexBuffer(mIndexBuffer);//设置顶点索引buffer数据
    bgfx::setState(state);//设置控制绘制信息的标志位
    bgfx::setTexture(0, mUniformTexture, mFirstInputFrameBuffer->getTexture());//设置对应的u_Texture这个着色器参数的纹理资源
    bgfx::submit(0, mProgram);//提交绘制单张纹理的Program
}

void XImageFilter::setVec4(std::string paramName, float *paramValue) {
    bgfx::UniformHandle handle = bgfx::createUniform(paramName.data(), bgfx::UniformType::Vec4);
    bgfx::setUniform(handle, paramValue);
    //todo: will be black and white first time if dstroy immediately.
//    bgfx::destroy(hanle);
}
NS_X_IMAGE_END