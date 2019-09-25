//
// Created by 林炳河 on 2019-09-25.
//

#include "XBlender.hpp"
#include "XImage.hpp"

NS_X_IMAGE_BEGIN
//顶点以及纹理坐标对象
struct PosTexVertexCopy
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

bgfx::VertexDecl PosTexVertexCopy::ms_decl;
//顶点坐标数据
PosTexVertexCopy s_vertices[] =
        {
                {-1.0f,  1.0f,  0.0f, 0.0f, 0.0f },
                { 1.0f,  1.0f,  0.0f, 1.0f, 0.0f },
                {-1.0f, -1.0f,  0.0f, 0.0f, 1.0f },
                { 1.0f, -1.0f,  0.0f, 1.0f, 1.0f },
        };
//顶点顺序
const uint16_t s_triList[] =
        {
                0, 1, 2, // 0
                1, 3, 2,
        };

void XBlender::blend(XFrameBuffer *bottom, XFrameBuffer *top) {
    bgfx::ProgramHandle program = loadProgram("vs_filter_normal", "fs_filter_normal");
    bgfx::UniformHandle texture = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    // Create vertex stream declaration.
    PosTexVertexCopy::init();

    // Create static vertex buffer.
    bgfx::VertexBufferHandle vertexBuffer = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_vertices, sizeof(s_vertices) )
            , PosTexVertexCopy::ms_decl);

    // Create static index buffer for triangle list rendering.
    bgfx::IndexBufferHandle indexBuffer = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(s_triList, sizeof(s_triList) ));
    int renderIndexBottom = XImage::nextRenderIndex();
    int renderIndexTop = XImage::nextRenderIndex();
    // Set view 0 default viewport.
    bgfx::setViewRect(renderIndexBottom, 0, 0, uint16_t(XImage::getCanvasWidth()), uint16_t(XImage::getCanvasHeight()) );
    bgfx::setViewRect(renderIndexTop, 0, 0, uint16_t(XImage::getCanvasWidth()), uint16_t(XImage::getCanvasHeight()) );
    // Set palette color for index 0
    bgfx::setPaletteColor(renderIndexBottom, 0.0f, 0.0f, 0.0f, 0.0f);
    // Set palette color for index 1
    bgfx::setPaletteColor(renderIndexTop, 1.0f, 1.0f, 1.0f, 1.0f);
    bgfx::setViewClear(renderIndexBottom
            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , 1.0f // Depth
            , 0    // Stencil
            , 0    // FB texture 0, color palette 0
            , 0 // FB texture 1, color palette 1
    );
    bgfx::setViewClear(renderIndexTop
            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , 1.0f // Depth
            , 0    // Stencil
            , 0    // Color palette 0
    );
    // Set vertex and index buffer.
    bgfx::setVertexBuffer(0, vertexBuffer);
    bgfx::setIndexBuffer(indexBuffer);
    const uint64_t state = 0
                           | BGFX_STATE_CULL_CW
                           | BGFX_STATE_WRITE_RGB
                           | BGFX_STATE_WRITE_A
                           | BGFX_STATE_DEPTH_TEST_LESS
                           | BGFX_STATE_MSAA
    ;
    const uint64_t stateNoDepth = 0
                                  | BGFX_STATE_CULL_CW
                                  | BGFX_STATE_WRITE_RGB
                                  | BGFX_STATE_WRITE_A
                                  | BGFX_STATE_DEPTH_TEST_ALWAYS
                                  | BGFX_STATE_MSAA
    ;
    // Set render states.
    bgfx::setState(state
                   | BGFX_STATE_BLEND_ALPHA
    );
    bgfx::setTexture(0, texture, bottom->getTexture());
    bgfx::submit(renderIndexBottom, program);
    // Set vertex and index buffer.
    bgfx::setVertexBuffer(0, vertexBuffer);
    bgfx::setIndexBuffer(indexBuffer);
    bgfx::setTexture(0, texture, top->getTexture());
    bgfx::submit(renderIndexTop, program);
}
NS_X_IMAGE_END