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

#include <iostream>

USING_NS_X_IMAGE

class ExampleFilters : public bigg::Application
{
	void initialize( int _argc, char** _argv )
	{

        mOutput = new XImagePictureOutput();
        mOutput->initWithPath("images/fengjing.jpg");
        mFilter = nullptr;
	}

	void onReset()
	{
		bgfx::setViewClear( 0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0 );
		bgfx::setViewRect( 0, 0, 0, uint16_t( getWidth() ), uint16_t( getHeight() ) );
	}

	void update( float dt )
	{
        mOutput->processPicture();
        
        ImGui::Begin("GPUImage-X");
        {
            ImGui::SetWindowSize(ImVec2(0, 0));
            const char* filterItems[] = { "None", "Saturation", "Contrast", "Brightness", "Exposure", "RGB", "HUE"};
            static int filterItemCurrent = 0;
            ImGui::Combo("Filter Selector",  &filterItemCurrent, filterItems, IM_ARRAYSIZE(filterItems), 5);
            {
                if (mCurrentIndex != filterItemCurrent) {
                    mOutput->clearTarget();
                    createAndAddFilter(filterItemCurrent);
                    mCurrentIndex = filterItemCurrent;
                }
                mOutput->notifyTargetsAboutNewOutputTexture();
            }
        }
        ImGui::End();
	}
    
    void createAndAddFilter(int index) {
        if (index == 0) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_normal");
        } else if (index == 1) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_saturation");
            mFilter->setFloat("saturation", 5.0);
        } else if (index == 2) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_contrast");
            mFilter->setFloat("contrast", 2.5);
        } else if (index == 3) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_brightness");
            mFilter->setFloat("brightness", 0.3);
        } else if (index == 4) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_exposure");
            mFilter->setFloat("exposure", -2.0);
        } else if (index == 5) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_rgb");
            mFilter->setFloat("redAdjustment", 0.3);
            mFilter->setFloat("greenAdjustment", 0.1);
            mFilter->setFloat("blueAdjustment", 0.5);
        } else if (index == 6) {
            mFilter = new XImageFilter("vs_filter_normal", "fs_filter_hue");
            mFilter->setFloat("hueAdjust", 5.0);
        }
       
        mOutput->addTarget(mFilter);
    }
public:
	ExampleFilters()
		: bigg::Application( "Filters", 960, 720 ) {}

private:
    XImagePictureOutput *mOutput;
    XImageFilter *mFilter;
    int mCurrentIndex = -1;
};

int main( int argc, char** argv )
{
	ExampleFilters app;
	return app.run( argc, argv );
}
