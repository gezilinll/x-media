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
                    mFilter = new XImageFilter("vs_filter_normal", FILTER_FRAGMENT_SHADERS[filterItemCurrent]);
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
            mFilter->setVec4("saturation", XImageUtils::wrapFloatToVec4(2.0 * ratio));
        } else if (strcmp("Contrast", type) == 0) {
            mFilter->setVec4("contrast", XImageUtils::wrapFloatToVec4(4.0 * ratio));
        } else if (strcmp("Brightness", type) == 0) {
            mFilter->setVec4("brightness", XImageUtils::wrapFloatToVec4(-1.0 + 2.0 * ratio));
        } else if (strcmp("Exposure", type) == 0) {
            mFilter->setVec4("exposure", XImageUtils::wrapFloatToVec4(-10.0 + 20.0 * ratio));
        } else if (strcmp("RGB", type) == 0) {
            mFilter->setVec4("redAdjustment", XImageUtils::wrapFloatToVec4(1.0 * ratio));
            mFilter->setVec4("greenAdjustment", XImageUtils::wrapFloatToVec4(1.0 * ratio));
            mFilter->setVec4("blueAdjustment", XImageUtils::wrapFloatToVec4(1.0 * ratio));
        } else if (strcmp("HUE", type) == 0) {
            mFilter->setVec4("hueAdjust", XImageUtils::wrapFloatToVec4(360 * ratio));
        } else if (strcmp("Levels", type) == 0) {
            mFilter->setVec4("levelMinimum", XImageUtils::wrapVec3ToVec4(0.0f, 0.0f, 0.0f));
            mFilter->setVec4("levelMiddle", XImageUtils::wrapVec3ToVec4(0.5f * ratio, 0.5f * ratio, 0.5f * ratio));
            mFilter->setVec4("levelMaximum", XImageUtils::wrapVec3ToVec4(1.0f * ratio, 1.0f * ratio, 1.0f * ratio));
            mFilter->setVec4("minOutput", XImageUtils::wrapVec3ToVec4(0.0f, 0.0f, 0.0f));
            mFilter->setVec4("maxOutput", XImageUtils::wrapVec3ToVec4(1.0f * ratio, 1.0f * ratio, 1.0f * ratio));
        } else if (strcmp("WhiteBalance", type) == 0) {
            mFilter->setVec4("temperature", XImageUtils::wrapFloatToVec4(10 * ratio));
            mFilter->setVec4("tint", XImageUtils::wrapFloatToVec4(5 * ratio));
        } else if (strcmp("Monochrome", type) == 0) {
            mFilter->setVec4("intensity", XImageUtils::wrapFloatToVec4(1.0 * ratio));
            mFilter->setVec4("filterColor", XImageUtils::wrapVec3ToVec4(0.6f * ratio, 0.3f * ratio, 0.8f * ratio));
        }
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
