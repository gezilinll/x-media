/*
 * Simple rotating cubes example, based off bgfx's example-01-cubes.
 * Does not mimic the original example completely because we're in a
 * different coordinate system.
 *
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "entry/entry.h"
#include "imgui/imgui.h"
#include "XImage.hpp"
#include "XFrameLayer.hpp"
#include "XFilterEffect.hpp"

USING_NS_X_IMAGE

class ExampleLayers : public entry::AppI {
public:
    ExampleLayers(const char* _name, const char* _description)
            : entry::AppI(_name, _description)
    {
    }

    void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
    {

        Args args(_argc, _argv);

        mWidth  = _width;
        mHeight = _height;

        bgfx::Init init;
        init.type     = args.m_type;
        init.vendorId = args.m_pciId;
        init.resolution.width  = mWidth;
        init.resolution.height = mHeight;
        init.resolution.reset  = BGFX_RESET_VSYNC;
        bgfx::init(init);

        m_debug = BGFX_DEBUG_TEXT;
        m_reset = BGFX_RESET_VSYNC;
        
        imguiCreate();

        mFrameLayer = new XFrameLayer(0);
        XRect layerRect = {0.0f, 0.0f, 720.0f, 1280.0f};
        mFrameLayer->setViewRect(layerRect);
        mFrameLayer->setPath("images/scene.jpg");
        mFilter = new XSaturation();
        mFilter->paramSaturationValue = 1.8;
        mFrameLayer->addEffect(mFilter);
    }

    virtual int shutdown() override
    {
        SAFE_DELETE(mFrameLayer);
        SAFE_DELETE(mFilter);

        imguiDestroy();

        // Shutdown bgfx.
        bgfx::shutdown();

        return 0;
    }

    bool update() override {
        if (!entry::processEvents(mWidth, mHeight, m_debug, m_reset, &m_mouseState) )
        {
            imguiBeginFrame(m_mouseState.m_mx
                            ,  m_mouseState.m_my
                            , (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
                            | (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
                            | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
                            ,  m_mouseState.m_mz
                            , uint16_t(mWidth)
                            , uint16_t(mHeight)
                            );
            ImGui::SetNextWindowPos(
                    ImVec2(mWidth - mWidth / 3.3f - 10.0f, 10.0f)
                    , ImGuiCond_FirstUseEver
            );
            ImGui::SetNextWindowSize(
                    ImVec2(mWidth / 3.3f, mWidth / 2.5f)
                    , ImGuiCond_FirstUseEver
            );
            ImGui::Begin("Settings"
                    , NULL
            );

            XImage::begin();

            mFrameLayer->submit();

            XImage::end();
            
            ImGui::End();
            imguiEndFrame();
            return true;
        }
        
        return false;
    }

private:
    entry::MouseState m_mouseState;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t m_debug;
    uint32_t m_reset;

    XFrameLayer *mFrameLayer;
    XSaturation *mFilter;
};


ENTRY_IMPLEMENT_MAIN(ExampleLayers, "layers", "layers xiuxiuxiu.");
