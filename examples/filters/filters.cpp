/*
 * Simple rotating cubes example, based off bgfx's example-01-cubes.
 * Does not mimic the original example completely because we're in a
 * different coordinate system.
 *
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bigg.hpp>
#include <bx/string.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "XImagePictureOutput.hpp"
#include "XImageFilter.hpp"
#include "XLog.hpp"
#include "XImageUtils.hpp"
#include "XImageShaderInfo.hpp"

#include <iostream>

USING_NS_X_IMAGE
const char *FILTER_ITEMS[] = {"None", "Saturation", "Contrast", "Brightness", "Exposure", "RGB", "HUE",
                             "Levels", "WhiteBalance", "Monochrome"};
const char *FILTER_FRAGMENT_SHADERS[] = {"fs_filter_normal", "fs_filter_saturation", "fs_filter_contrast",
                                         "fs_filter_brightness", "fs_filter_exposure", "fs_filter_rgb", "fs_filter_hue",
                                         "fs_filter_levels", "fs_filter_white_balance", "fs_filter_monochrome"};
class ExampleFilters : public bigg::Application {
    void initialize(int _argc, char **_argv) {

        mOutput = new XImagePictureOutput();
        mOutput->initWithPath("images/fengjing.jpg");
        mFilter = nullptr;
        mRatio = 0.0f;
        mOffset = 0.001f;
    }

    void onReset() {
        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, uint16_t(getWidth()), uint16_t(getHeight()));
    }

    int shutdown() {
        SAFE_DELETE(mOutput);
        SAFE_DELETE(mFilter);
    }

    void update(float dt) {
        mRatio = mRatio + mOffset;
        if (mRatio > 1.0f) {
            mOffset = -0.001;
            mRatio = 1.0f;
        } else if (mRatio < 0.0f) {
            mOffset = 0.001;
            mRatio = 0.0f;
        }

        mOutput->processPicture();

        ImGui::Begin("GPUImage-X");
        {
            ImGui::SetWindowSize(ImVec2(0, 0));
            static int filterItemCurrent = 0;
            ImGui::Combo("Filter Selector", &filterItemCurrent, FILTER_ITEMS, IM_ARRAYSIZE(FILTER_ITEMS), 5);
            {
                if (mCurrentIndex != filterItemCurrent) {
                    mOutput->clearTarget();
                    SAFE_DELETE(mFilter);
                    mFilter = new XImageFilter(getShaderPath("vs_filter_normal"),
                            getShaderPath(FILTER_FRAGMENT_SHADERS[filterItemCurrent]));
                    mFilter->setViewRect(0, 0, 960, 720);
                    mOutput->addTarget(mFilter);
                    mCurrentIndex = filterItemCurrent;
                }
                updateFilter(filterItemCurrent, mRatio);
                mOutput->notifyTargetsAboutNewOutputTexture();
            }
        }
        ImGui::End();
    }

    void updateFilter(int index, float ratio) {
        const char *type = FILTER_ITEMS[index];
        if (strcmp("None", type) == 0) {
        } else if (strcmp("Saturation", type) == 0) {
            Saturation saturation;
            mFilter->setVec4(saturation.paramName, XImageUtils::wrapFloatToVec4(saturation.paramMax * ratio));
        } else if (strcmp("Contrast", type) == 0) {
            Contrast contrast;
            mFilter->setVec4(contrast.paramName, XImageUtils::wrapFloatToVec4(contrast.paramMax * ratio));
        } else if (strcmp("Brightness", type) == 0) {
            Brightness brightness;
            mFilter->setVec4(brightness.paramName,
                             XImageUtils::wrapFloatToVec4(brightness.paramMin + 2 * brightness.paramMax * ratio));
        } else if (strcmp("Exposure", type) == 0) {
            Exposure exposure;
            mFilter->setVec4(exposure.paramName,
                             XImageUtils::wrapFloatToVec4(exposure.paramMin + 2 * exposure.paramMax * ratio));
        } else if (strcmp("RGB", type) == 0) {
            RGB rgb;
            mFilter->setVec4(rgb.paramRedName, XImageUtils::wrapFloatToVec4(2.0 * ratio));
            mFilter->setVec4(rgb.paramGreenName, XImageUtils::wrapFloatToVec4(2.0 * ratio));
            mFilter->setVec4(rgb.paramBlueName, XImageUtils::wrapFloatToVec4(2.0 * ratio));
        } else if (strcmp("HUE", type) == 0) {
            HUE hue;
            mFilter->setVec4(hue.paramName, XImageUtils::wrapFloatToVec4(hue.paramMax * ratio));
        } else if (strcmp("Levels", type) == 0) {
            Levels levels;
            mFilter->setVec4(levels.paramMinLevelName, XImageUtils::wrapVec3ToVec4(levels.paramMin));
            mFilter->setVec4(levels.paramMiddleLevelName,
                             XImageUtils::wrapVec3ToVec4(2.0f * ratio, 2.0f * ratio, 2.0f * ratio));
            mFilter->setVec4(levels.paramMaxLevelName, XImageUtils::wrapVec3ToVec4(levels.paramMax[0] * ratio,
                                                                                   levels.paramMax[1] * ratio,
                                                                                   levels.paramMax[2] * ratio));
            mFilter->setVec4(levels.paramMinOutName, XImageUtils::wrapVec3ToVec4(levels.paramMin));
            mFilter->setVec4(levels.paramMaxOutName, XImageUtils::wrapVec3ToVec4(levels.paramMax[0] * ratio,
                                                                                 levels.paramMax[1] * ratio,
                                                                                 levels.paramMax[2] * ratio));
        } else if (strcmp("WhiteBalance", type) == 0) {
            WhiteBalance whiteBalance;
            mFilter->setVec4(whiteBalance.paramTemperatureName, XImageUtils::wrapFloatToVec4(10 * ratio));
            mFilter->setVec4(whiteBalance.paramTintName, XImageUtils::wrapFloatToVec4(5 * ratio));
        } else if (strcmp("Monochrome", type) == 0) {
            Monochrome monochrome;
            mFilter->setVec4(monochrome.paramIntensityName,
                             XImageUtils::wrapFloatToVec4(monochrome.paramIntensityMax * ratio));
            mFilter->setVec4(monochrome.paramFilterColorName,
                             XImageUtils::wrapVec3ToVec4(monochrome.paramFilterColorMax[0] * ratio,
                                                         monochrome.paramFilterColorMax[1] * ratio,
                                                         monochrome.paramFilterColorMax[2] * ratio));
        }
    }

    char* getShaderPath(const char* shaderName) {
        char filePath[512];

        const char *shaderPath = "???";

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

        bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
        bx::strCat(filePath, BX_COUNTOF(filePath), shaderName);
        bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

        return filePath;
    }

public:
    ExampleFilters()
            : bigg::Application("Filters", 960, 720) {}

private:
    XImagePictureOutput *mOutput;
    XImageFilter *mFilter;
    int mCurrentIndex = -1;
    float mRatio;
    float mOffset;
};

int main(int argc, char **argv) {
    ExampleFilters app;
    return app.run(argc, argv);
}
