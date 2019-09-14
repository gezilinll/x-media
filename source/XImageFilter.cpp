//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFilter.hpp"
#include "XImage.hpp"
#include "XLog.hpp"

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
    mVertexBuffer = BGFX_INVALID_HANDLE;
    mIndexBuffer = BGFX_INVALID_HANDLE;

    mViewRectX = 0;
    mViewRectY = 0;
    mViewRectWidth = 0;
    mViewRectHeight = 0;
}

XImageFilter::~XImageFilter() {
    XImage::destroy(mProgram);
    XImage::destroy(mVertexBuffer);
    XImage::destroy(mIndexBuffer);

    auto iter = mUniformHandles.begin();
    while (iter != mUniformHandles.end()) {
        XImage::destroy(iter->second);
        iter++;
    }
    mUniformHandles.clear();
}

void XImageFilter::setInputFrameBuffer(XImageFrameBuffer *input) {
    mFirstInputFrameBuffer = input;
}

void XImageFilter::setViewRect(int x, int y, int width, int height) {
    mViewRectX = x;
    mViewRectY = y;
    mViewRectWidth = width;
    mViewRectHeight = height;
}

void XImageFilter::renderAtProgress(float progress) {
    if (!isViewRectValid()) {
        LOGE("XImageFilter::renderAtProgress view rect args is invalid.");
        return;
    }
    if (mFirstInputFrameBuffer == nullptr || !bgfx::isValid(mFirstInputFrameBuffer->getTexture())) {
        LOGE("XImageFilter::renderAtProgress intput frame is invalid.");
        return;
    }
    if (!bgfx::isValid(mProgram)) {
        mProgram = loadProgram(mVertexShaderPath.data(), mFragmentShaderPath.data());
        bgfx::UniformHandle texture = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
        mUniformHandles.insert(std::make_pair("s_texColor", texture));
        // Create vertex stream declaration.
        PosTexVertex::init();

        // Create static vertex buffer.
        mVertexBuffer = bgfx::createVertexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_vertices, sizeof(s_vertices) )
                , PosTexVertex::ms_decl);

        // Create static index buffer for triangle list rendering.
        mIndexBuffer = bgfx::createIndexBuffer(
                // Static data can be passed with bgfx::makeRef
                bgfx::makeRef(s_triList, sizeof(s_triList) ));
    }
    
    if (!bgfx::isValid(mProgram)) {
        LOGE("XImageFilter::renderAtProgress load program failed. vsPath=%s, fsPath=%s", mVertexShaderPath.data(), mFragmentShaderPath.data());
        return;
    }

    int renderIndex = XImage::renderIndex();
    if (!mTargets.empty()) {
        if (!bgfx::isValid(mOutputFrameBuffer->get())) {
            mOutputFrameBuffer->create(mViewRectWidth, mViewRectHeight, bgfx::TextureFormat::BGRA8);
        }
        bgfx::setViewFrameBuffer(renderIndex, mOutputFrameBuffer->get());
    } else {
        bgfx::setViewFrameBuffer(renderIndex, BGFX_INVALID_HANDLE);
    }
    bgfx::setViewClear(renderIndex
            , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
            , 0x303030ff
            , 1.0f
            , 0);
    uint64_t state = 0
                     | BGFX_STATE_WRITE_R
                     | BGFX_STATE_WRITE_G
                     | BGFX_STATE_WRITE_B
                     | BGFX_STATE_WRITE_A
                     | UINT64_C(0);
    bgfx::touch(renderIndex);
    bgfx::setViewRect(renderIndex, mViewRectX, mViewRectY, mViewRectWidth, mViewRectHeight);
    bgfx::setVertexBuffer(0, mVertexBuffer);
    bgfx::setIndexBuffer(mIndexBuffer);
    bgfx::setState(state);
    bgfx::setTexture(0, mUniformHandles.find("s_texColor")->second, mFirstInputFrameBuffer->getTexture());
    bgfx::submit(renderIndex, mProgram);

    renderTargetsByNewOutputTexture(progress);
}

void XImageFilter::setVec4(std::string paramName, float *paramValue) {
    bgfx::UniformHandle handle;
    auto iter = mUniformHandles.find(paramName);
    if (iter == mUniformHandles.end()) {
        handle = bgfx::createUniform(paramName.data(), bgfx::UniformType::Vec4);
        mUniformHandles.insert(std::make_pair(paramName, handle));
    } else {
        handle = iter->second;
    }
    if (!bgfx::isValid(handle)) {
        LOGE("XImageFilter::setVec4 uniform handle is invalid.");
    }
    bgfx::setUniform(handle, paramValue);
}

bool XImageFilter::isViewRectValid() {
    return mViewRectWidth > 0 && mViewRectHeight > 0;
}
NS_X_IMAGE_END
