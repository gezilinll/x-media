#include "entry/entry.h"
#include "imgui/imgui.h"
#include "XImage.hpp"
#include "XFrameLayer.hpp"
#include "XEffectListUI.hpp"

USING_NS_X_IMAGE
class ExampleLayers : public entry::AppI {
public:
    ExampleLayers(const char* _name, const char* _description)
            : entry::AppI(_name, _description) {}

    void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override {
        // 初始化
        Args args(_argc, _argv);
        mWidth  = _width;
        mHeight = _height;
        mDebug = BGFX_DEBUG_TEXT;
        mReset = BGFX_RESET_VSYNC;
        bgfx::Init init;
        init.type     = args.m_type;
        init.vendorId = args.m_pciId;
        init.resolution.width  = mWidth;
        init.resolution.height = mHeight;
        init.resolution.reset  = BGFX_RESET_VSYNC;
        XImage::init(init);

        mHorizontalMargin = 10;
        mVerticalMargin = 10;
        mMenuWidth = mWidth / 4;
        float ratio = 1920.0f / 1080.0f;
        mLayerWidth = (mWidth - mMenuWidth - 3 * mHorizontalMargin) / 2;
        mLayerHeight = (mHeight - 3 * mVerticalMargin) / 2;
        if (mLayerWidth / mLayerHeight > ratio) {
            mLayerWidth = mLayerHeight * ratio;
        } else if (mLayerWidth / mLayerHeight < ratio) {
            mLayerHeight = mLayerWidth / ratio;
        }
        mHorizontalMargin = (mWidth - 2 * mLayerWidth - mMenuWidth) / 3;
        mVerticalMargin = (mHeight - 2 * mLayerHeight) / 3;

        // 创建调试ui
        imguiCreate();

        // 初始化图层与默认滤镜
        mEffectListUI = new XEffectListUI();
        mDefaultFilters.push_back(new XFilterEffect());
        mDefaultFilters.push_back(new XFilterEffect());
        mDefaultFilters.push_back(new XFilterEffect());
        mDefaultFilters.push_back(new XFilterEffect());

        mFrameLayers.push_back(new XFrameLayer(0));
        XRect leftTop = {static_cast<int>(mHorizontalMargin), static_cast<int>(mVerticalMargin),
                         static_cast<unsigned int>(mLayerWidth), static_cast<unsigned int>(mLayerHeight)};
        mFrameLayers[0]->setViewRect(leftTop);
        mFrameLayers[0]->setPath("images/spring.jpg");
        mFrameLayers[0]->addEffect(mDefaultFilters[0]);

        mFrameLayers.push_back(new XFrameLayer(1));
        XRect rightTop = {static_cast<int>(mLayerWidth + mHorizontalMargin * 2), static_cast<int>(mVerticalMargin),
                          static_cast<unsigned int>(mLayerWidth), static_cast<unsigned int>(mLayerHeight)};
        mFrameLayers[1]->setViewRect(rightTop);
        mFrameLayers[1]->setPath("images/summer.jpg");
        mFrameLayers[1]->addEffect(mDefaultFilters[1]);

        mFrameLayers.push_back(new XFrameLayer(2));
        XRect leftBottom = {static_cast<int>(mHorizontalMargin), static_cast<int>(mLayerHeight + mVerticalMargin * 2),
                            static_cast<unsigned int>(mLayerWidth), static_cast<unsigned int>(mLayerHeight)};
        mFrameLayers[2]->setViewRect(leftBottom);
        mFrameLayers[2]->setPath("images/autumn.jpg");
        mFrameLayers[2]->addEffect(mDefaultFilters[2]);

        mFrameLayers.push_back(new XFrameLayer(3));
        XRect rightBottom = {static_cast<int>(mLayerWidth + mHorizontalMargin * 2),
                             static_cast<int>(mLayerHeight + mVerticalMargin * 2),
                             static_cast<unsigned int>(mLayerWidth), static_cast<unsigned int>(mLayerHeight)};
        mFrameLayers[3]->setViewRect(rightBottom);
        mFrameLayers[3]->setPath("images/winter.jpg");
        mFrameLayers[3]->addEffect(mDefaultFilters[3]);
        XImage::addLayer(mFrameLayers[0]);
        XImage::addLayer(mFrameLayers[1]);
        XImage::addLayer(mFrameLayers[2]);
        XImage::addLayer(mFrameLayers[3]);
    }

    virtual int shutdown() override {
        imguiDestroy();

        XImage::destroy();

        return 0;
    }

    bool update() override {
        if (!entry::processEvents(mWidth, mHeight, mDebug, mReset, &mMouseState)) {
            // 刷新调试界面
            imguiBeginFrame(mMouseState.m_mx
                            ,  mMouseState.m_my
                            , (mMouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
                            | (mMouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
                            | (mMouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
                            ,  mMouseState.m_mz
                            , uint16_t(mWidth)
                            , uint16_t(mHeight)
                            );
            ImGui::SetNextWindowPos(
                    ImVec2(mHorizontalMargin * 3 + mLayerWidth * 2, 0)
                    , ImGuiCond_FirstUseEver
            );
            ImGui::SetNextWindowSize(
                    ImVec2(mMenuWidth, mHeight)
                    , ImGuiCond_FirstUseEver
            );
            ImGui::Begin("Settings"
                    , NULL
            );

            ImGui::Text("Layer:");
            static int currentEmitter = 0;
            for (uint32_t ii = 0; ii < mFrameLayers.size(); ++ii) {
                ImGui::SameLine();
                char name[16];
                bx::snprintf(name, BX_COUNTOF(name), "%d", ii);
                ImGui::RadioButton(name, &mCurrentLayer, ii);
            }
            mEffectListUI->imgui();
            XEffectUI *newEffectUI = mEffectListUI->newEffectUI();
            if (newEffectUI != nullptr) {
                mEffectUIs[mFrameLayers[mCurrentLayer]->getID()].push_back(newEffectUI);
                mFrameLayers[mCurrentLayer]->addEffect(newEffectUI->getEffect());
            }
            std::vector<XEffectUI *> effectUIs = mEffectUIs[mFrameLayers[mCurrentLayer]->getID()];
            int effectIndex = 0;
            for (XEffectUI *effectUI : effectUIs) {
                effectUI->setIndex(effectIndex);
                effectUI->imgui();
                effectUI->update();
                effectIndex++;
            }

            ImGui::End();
            imguiEndFrame();

            // 刷新图层及效果
            XImage::begin();
            XImage::submit();
            XImage::frame();
            XImage::end();

            return true;
        }
        
        return false;
    }

private:
    entry::MouseState mMouseState;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mLayerWidth;
    uint32_t mLayerHeight;
    uint32_t mMenuWidth;
    uint32_t mHorizontalMargin;
    uint32_t mVerticalMargin;
    uint32_t mDebug;
    uint32_t mReset;

    XEffectListUI *mEffectListUI;
    std::vector<XFilterEffect *> mDefaultFilters;
    std::vector<XFrameLayer *> mFrameLayers;
    std::unordered_map<int, std::vector<XEffectUI *>> mEffectUIs;
    int mCurrentLayer = 0;
};


ENTRY_IMPLEMENT_MAIN(ExampleLayers, "layers", "layers xiuxiuxiu.");
