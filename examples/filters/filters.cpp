/*
 * Simple rotating cubes example, based off bgfx's example-01-cubes.
 * Does not mimic the original example completely because we're in a
 * different coordinate system.
 *
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bx/string.h>
#include "XImagePictureOutput.hpp"
#include "XImageFilter.hpp"
#include "XLog.hpp"
#include "XImageUtils.hpp"
#include "XImageShaderInfo.hpp"
#include "entry/entry.h"
#include "imgui/imgui.h"
#include "bgfx_utils.h"
#include <unordered_map>
#include <iostream>

USING_NS_X_IMAGE
const char *FILTER_ITEMS[] = {"None", "Saturation", "Contrast", "Brightness", "Exposure", "RGB", "HUE",
                             "Levels", "WhiteBalance", "Monochrome"};
const char *FILTER_FRAGMENT_SHADERS[] = {"fs_filter_normal", "fs_filter_saturation", "fs_filter_contrast",
                                         "fs_filter_brightness", "fs_filter_exposure", "fs_filter_rgb", "fs_filter_hue",
                                         "fs_filter_levels", "fs_filter_white_balance", "fs_filter_monochrome"};
class ExampleFilters : public entry::AppI {
public:
    ExampleFilters(const char* _name, const char* _description)
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
        
        // Set view 0 clear state.
        bgfx::setViewClear(0
                , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                , 0x303030ff
                , 1.0f
                , 0
        );

        imguiCreate();

        mOutput = new XImagePictureOutput();
        mOutput->initWithPath("/Users/linbinghe/Projects/GPUImage-X/examples/runtime/images/fengjing.jpg");
        mFilter = nullptr;
        mRatio = 0.0f;
        mOffset = 0.001f;
    }

    virtual int shutdown() override
    {
        SAFE_DELETE(mOutput);
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

            mRatio = mRatio + mOffset;
            if (mRatio > 1.0f) {
                mOffset = -0.001;
                mRatio = 1.0f;
            } else if (mRatio < 0.0f) {
                mOffset = 0.001;
                mRatio = 0.0f;
            }
            
            static int filterItemCurrent = 0;
            ImGui::Combo("Filter Selected", &filterItemCurrent, FILTER_ITEMS, IM_ARRAYSIZE(FILTER_ITEMS), 5);
            {
                if (mCurrentIndex != filterItemCurrent) {
                    mOutput->clearTarget();
                    SAFE_DELETE(mFilter);
                    mFilter = new XImageFilter(getShaderPath("vs_filter_normal"),
                                               getShaderPath(FILTER_FRAGMENT_SHADERS[filterItemCurrent]));
                    mFilter->setViewRect(0, 0, mWidth, mHeight);
                    mOutput->addTarget(mFilter);
                    mCurrentIndex = filterItemCurrent;
                    updateFilterSettings(filterItemCurrent, mRatio, false);
                } else {
                    updateFilterSettings(filterItemCurrent, mRatio, true);
                }

                mOutput->notifyTargetsAboutNewOutputTexture();
            }

            ImGui::End();
            imguiEndFrame();
            
            mOutput->processPicture();
           
            // Advance to next frame. Rendering thread will be kicked to
            // process submitted rendering primitives.
            bgfx::frame();
            
            return true;
        }
        
        return false;
    }

    void updateFilterSettings(int index, float ratio, bool updateOnly) {
        ImGui::Separator();
        const char *type = FILTER_ITEMS[index];
        if (strcmp("None", type) == 0) {
        } else if (strcmp("Saturation", type) == 0) {
            Saturation saturation;
            if (!updateOnly) {
                mValues[saturation.paramName] = saturation.paramNormal;
            }
            ImGui::SliderFloat(saturation.paramName.data()
                    , &mValues[saturation.paramName]
                    , saturation.paramMin
                    , saturation.paramMax
            );
            mFilter->setVec4(saturation.paramName, XImageUtils::wrapFloatToVec4(mValues[saturation.paramName]));
        } else if (strcmp("Contrast", type) == 0) {
            Contrast contrast;
            if (!updateOnly) {
                mValues[contrast.paramName] = contrast.paramNormal;
            }
            ImGui::SliderFloat(contrast.paramName.data()
                    , &mValues[contrast.paramName]
                    , contrast.paramMin
                    , contrast.paramMax
            );
            mFilter->setVec4(contrast.paramName, XImageUtils::wrapFloatToVec4(mValues[contrast.paramName]));
        } else if (strcmp("Brightness", type) == 0) {
            Brightness brightness;
            if (!updateOnly) {
                mValues[brightness.paramName] = brightness.paramNormal;
            }
            ImGui::SliderFloat(brightness.paramName.data()
                    , &mValues[brightness.paramName]
                    , brightness.paramMin
                    , brightness.paramMax
            );
            mFilter->setVec4(brightness.paramName,
                             XImageUtils::wrapFloatToVec4(mValues[brightness.paramName]));
        } else if (strcmp("Exposure", type) == 0) {
            Exposure exposure;
            if (!updateOnly) {
                mValues[exposure.paramName] = exposure.paramNormal;
            }
            ImGui::SliderFloat(exposure.paramName.data()
                    , &mValues[exposure.paramName]
                    , exposure.paramMin
                    , exposure.paramMax
            );
            mFilter->setVec4(exposure.paramName,
                             XImageUtils::wrapFloatToVec4(mValues[exposure.paramName]));
        } else if (strcmp("RGB", type) == 0) {
            RGB rgb;
            if (!updateOnly) {
                mValues[rgb.paramRedName] = rgb.paramNormal;
                mValues[rgb.paramBlueName] = rgb.paramNormal;
                mValues[rgb.paramGreenName] = rgb.paramNormal;
            }
            ImGui::SliderFloat(rgb.paramRedName.data()
                    , &mValues[rgb.paramRedName]
                    , rgb.paramMin
                    , 10.0f
            );
            ImGui::SliderFloat(rgb.paramGreenName.data()
                    , &mValues[rgb.paramGreenName]
                    , rgb.paramMin
                    , 10.0f
            );
            ImGui::SliderFloat(rgb.paramBlueName.data()
                    , &mValues[rgb.paramBlueName]
                    , rgb.paramMin
                    , 10.0f
            );
            mFilter->setVec4(rgb.paramRedName, XImageUtils::wrapFloatToVec4(mValues[rgb.paramRedName]));
            mFilter->setVec4(rgb.paramGreenName, XImageUtils::wrapFloatToVec4(mValues[rgb.paramRedName]));
            mFilter->setVec4(rgb.paramBlueName, XImageUtils::wrapFloatToVec4(mValues[rgb.paramBlueName]));
        } else if (strcmp("HUE", type) == 0) {
            HUE hue;
            if (!updateOnly) {
                mValues[hue.paramName] = hue.paramNormal;
            }
            ImGui::SliderFloat(hue.paramName.data()
                    , &mValues[hue.paramName]
                    , hue.paramMin
                    , hue.paramMax
            );
            mFilter->setVec4(hue.paramName, XImageUtils::wrapFloatToVec4(mValues[hue.paramName]));
        } else if (strcmp("Levels", type) == 0) {
            Levels levels;
            if (!updateOnly) {
                mValues[levels.paramMinLevelName + "R"] = levels.paramMinNormal[0];
                mValues[levels.paramMinLevelName + "G"] = levels.paramMinNormal[1];
                mValues[levels.paramMinLevelName + "B"] = levels.paramMinNormal[2];
                mValues[levels.paramMaxLevelName + "R"] = levels.paramMaxNormal[0];
                mValues[levels.paramMaxLevelName + "G"] = levels.paramMaxNormal[1];
                mValues[levels.paramMaxLevelName + "B"] = levels.paramMaxNormal[2];
                mValues[levels.paramMiddleLevelName + "R"] = levels.paramMiddleNormal[0];
                mValues[levels.paramMiddleLevelName + "G"] = levels.paramMiddleNormal[1];
                mValues[levels.paramMiddleLevelName + "B"] = levels.paramMiddleNormal[2];
                mValues[levels.paramMinOutName + "R"] = levels.paramMinNormal[0];
                mValues[levels.paramMinOutName + "G"] = levels.paramMinNormal[1];
                mValues[levels.paramMinOutName + "B"] = levels.paramMinNormal[2];
                mValues[levels.paramMaxOutName + "R"] = levels.paramMaxNormal[0];
                mValues[levels.paramMaxOutName + "G"] = levels.paramMaxNormal[1];
                mValues[levels.paramMaxOutName + "B"] = levels.paramMaxNormal[2];
            }
            ImGui::SliderFloat((levels.paramMinLevelName + "R").data()
                    , &mValues[levels.paramMinLevelName + "R"]
                    , levels.paramMin[0]
                    , levels.paramMax[0]
            );
            ImGui::SliderFloat((levels.paramMinLevelName + "G").data()
                    , &mValues[levels.paramMinLevelName + "G"]
                    , levels.paramMin[1]
                    , levels.paramMax[1]
            );
            ImGui::SliderFloat((levels.paramMinLevelName + "B").data()
                    , &mValues[levels.paramMinLevelName + "B"]
                    , levels.paramMin[2]
                    , levels.paramMax[2]
            );
            ImGui::Separator();
            ImGui::SliderFloat((levels.paramMaxLevelName + "R").data()
                    , &mValues[levels.paramMaxLevelName + "R"]
                    , levels.paramMin[0]
                    , levels.paramMax[0]
            );
            ImGui::SliderFloat((levels.paramMaxLevelName + "G").data()
                    , &mValues[levels.paramMaxLevelName + "G"]
                    , levels.paramMin[1]
                    , levels.paramMax[1]
            );
            ImGui::SliderFloat((levels.paramMaxLevelName + "B").data()
                    , &mValues[levels.paramMaxLevelName + "B"]
                    , levels.paramMin[2]
                    , levels.paramMax[2]
            );
            ImGui::Separator();
            ImGui::SliderFloat((levels.paramMiddleLevelName + "R").data()
                    , &mValues[levels.paramMiddleLevelName + "R"]
                    , levels.paramMin[0]
                    , 100.0f
            );
            ImGui::SliderFloat((levels.paramMiddleLevelName + "G").data()
                    , &mValues[levels.paramMiddleLevelName + "G"]
                    , levels.paramMin[1]
                    , 100.0f
            );
            ImGui::SliderFloat((levels.paramMiddleLevelName + "B").data()
                    , &mValues[levels.paramMiddleLevelName + "B"]
                    , levels.paramMin[2]
                    , 100.0f
            );
            ImGui::Separator();
            ImGui::SliderFloat((levels.paramMinOutName + "R").data()
                    , &mValues[levels.paramMinOutName + "R"]
                    , levels.paramMin[0]
                    , levels.paramMax[0]
            );
            ImGui::SliderFloat((levels.paramMinOutName + "G").data()
                    , &mValues[levels.paramMinOutName + "G"]
                    , levels.paramMin[1]
                    , levels.paramMax[1]
            );
            ImGui::SliderFloat((levels.paramMinOutName + "B").data()
                    , &mValues[levels.paramMinOutName + "B"]
                    , levels.paramMin[2]
                    , levels.paramMax[2]
            );
            ImGui::Separator();
            ImGui::SliderFloat((levels.paramMaxOutName + "R").data()
                    , &mValues[levels.paramMaxOutName + "R"]
                    , levels.paramMin[0]
                    , levels.paramMax[0]
            );
            ImGui::SliderFloat((levels.paramMaxOutName + "G").data()
                    , &mValues[levels.paramMaxOutName + "G"]
                    , levels.paramMin[1]
                    , levels.paramMax[1]
            );
            ImGui::SliderFloat((levels.paramMaxOutName + "B").data()
                    , &mValues[levels.paramMaxOutName + "B"]
                    , levels.paramMin[2]
                    , levels.paramMax[2]
            );
            mFilter->setVec4(levels.paramMinLevelName,
                             XImageUtils::wrapVec3ToVec4(mValues[levels.paramMinLevelName + "R"],
                                                         mValues[levels.paramMinLevelName + "G"],
                                                         mValues[levels.paramMinLevelName + "B"]));
            mFilter->setVec4(levels.paramMiddleLevelName,
                             XImageUtils::wrapVec3ToVec4(mValues[levels.paramMiddleLevelName + "R"],
                                                         mValues[levels.paramMiddleLevelName + "G"],
                                                         mValues[levels.paramMiddleLevelName + "B"]));
            mFilter->setVec4(levels.paramMaxLevelName,
                             XImageUtils::wrapVec3ToVec4(mValues[levels.paramMaxLevelName + "R"],
                                                         mValues[levels.paramMaxLevelName + "G"],
                                                         mValues[levels.paramMaxLevelName + "B"]));
            mFilter->setVec4(levels.paramMinOutName,
                             XImageUtils::wrapVec3ToVec4(mValues[levels.paramMinOutName + "R"],
                                                         mValues[levels.paramMinOutName + "G"],
                                                         mValues[levels.paramMinOutName + "B"]));
            mFilter->setVec4(levels.paramMaxOutName,
                             XImageUtils::wrapVec3ToVec4(mValues[levels.paramMaxOutName + "R"],
                                                         mValues[levels.paramMaxOutName + "G"],
                                                         mValues[levels.paramMaxOutName + "B"]));
        } else if (strcmp("WhiteBalance", type) == 0) {
            WhiteBalance whiteBalance;
            if (!updateOnly) {
                mValues[whiteBalance.paramTemperatureName] = whiteBalance.paramTemperatureDefault;
                mValues[whiteBalance.paramTintName] = whiteBalance.paramTintDefault;
            }
            ImGui::SliderFloat(whiteBalance.paramTemperatureName.data()
                    , &mValues[whiteBalance.paramTemperatureName]
                    , -50
                    , 50
            );
            ImGui::SliderFloat(whiteBalance.paramTintName.data()
                    , &mValues[whiteBalance.paramTintName]
                    , -10
                    , 10
            );
            mFilter->setVec4(whiteBalance.paramTemperatureName,
                             XImageUtils::wrapFloatToVec4(mValues[whiteBalance.paramTemperatureName]));
            mFilter->setVec4(whiteBalance.paramTintName,
                             XImageUtils::wrapFloatToVec4(mValues[whiteBalance.paramTintName]));
        } else if (strcmp("Monochrome", type) == 0) {
            Monochrome monochrome;
            if (!updateOnly) {
                mValues[monochrome.paramIntensityName] = monochrome.paramIntensityNormal;
                mValues[monochrome.paramFilterColorName + "R"] = 0.5;
                mValues[monochrome.paramFilterColorName + "G"] = 0.5;
                mValues[monochrome.paramFilterColorName + "B"] = 0.5;
            }
            ImGui::SliderFloat(monochrome.paramIntensityName.data()
                    , &mValues[monochrome.paramIntensityName]
                    , monochrome.paramIntensityMin
                    , monochrome.paramIntensityMax
            );
            ImGui::Separator();
            ImGui::SliderFloat((monochrome.paramFilterColorName + "R").data()
                    , &mValues[monochrome.paramFilterColorName + "R"]
                    , monochrome.paramFilterColorMin[0]
                    , monochrome.paramFilterColorMax[0]
            );
            ImGui::SliderFloat((monochrome.paramFilterColorName + "G").data()
                    , &mValues[monochrome.paramFilterColorName + "G"]
                    , monochrome.paramFilterColorMin[1]
                    , monochrome.paramFilterColorMax[1]
            );
            ImGui::SliderFloat((monochrome.paramFilterColorName + "B").data()
                    , &mValues[monochrome.paramFilterColorName + "B"]
                    , monochrome.paramFilterColorMin[2]
                    , monochrome.paramFilterColorMax[2]
            );
            mFilter->setVec4(monochrome.paramIntensityName,
                             XImageUtils::wrapFloatToVec4(mValues[monochrome.paramIntensityName]));
            mFilter->setVec4(monochrome.paramFilterColorName,
                             XImageUtils::wrapVec3ToVec4(mValues[monochrome.paramFilterColorName + "R"],
                                                         mValues[monochrome.paramFilterColorName + "G"],
                                                         mValues[monochrome.paramFilterColorName + "B"]));
        }
    }

    std::string getShaderPath(const char* shaderName) {
        
        std::string shaderPath = "";

        switch (bgfx::getRendererType()) {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D9:
                shaderPath = "shaders/dx9/";
                break;
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12:
                shaderPath = "shaders/dx11/";
                break;
            case bgfx::RendererType::Gnm:
                shaderPath = "shaders/pssl/";
                break;
            case bgfx::RendererType::Metal:
                shaderPath = "shaders/metal/";
                break;
            case bgfx::RendererType::Nvn:
                shaderPath = "shaders/nvn/";
                break;
            case bgfx::RendererType::OpenGL:
                shaderPath = "shaders/glsl/";
                break;
            case bgfx::RendererType::OpenGLES:
                shaderPath = "shaders/essl/";
                break;
            case bgfx::RendererType::Vulkan:
                shaderPath = "shaders/spirv/";
                break;

            case bgfx::RendererType::Count:
                BX_CHECK(false, "You should not be here!");
                break;
        }

        return "/Users/linbinghe/Projects/GPUImage-X/examples/runtime/" + shaderPath + shaderName + ".bin";
    }

private:
    entry::MouseState m_mouseState;
    XImagePictureOutput *mOutput;
    XImageFilter *mFilter;
    int mCurrentIndex = -1;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t m_debug;
    uint32_t m_reset;
    float mRatio;
    float mOffset;
    std::unordered_map<std::string, Shader> mShaders;
    std::unordered_map<std::string, float > mValues;
};


ENTRY_IMPLEMENT_MAIN(ExampleFilters, "filters", "filter xiuxiuxiu.");
