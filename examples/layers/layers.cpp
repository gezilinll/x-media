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

        // 创建调试ui
        imguiCreate();

        // 初始化图层与滤镜
        mEffectListUI = new XEffectListUI();
        mFrameLayers.push_back(new XFrameLayer(0));
        XRect layerRect = {0, 0, mWidth, mHeight};
        mFrameLayers[0]->setViewRect(layerRect);
        mFrameLayers[0]->setPath("images/scene.jpg");
        mFilter = new XSaturation();
        mFrameLayers[0]->addEffect(mFilter);
        XImage::addLayer(mFrameLayers[0]);
        XSaturationUI *saturationUI = new XSaturationUI(mFilter);
        mEffectUIs[mFrameLayers[0]->getID()].push_back(saturationUI);
    }

    virtual int shutdown() override {
        SAFE_DELETE(mFilter);

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
                LOGE("lbh newEffectUI != nullptr");
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
            XImage::end();

            return true;
        }
        
        return false;
    }

private:
    entry::MouseState mMouseState;
    uint32_t mWidth;
    uint32_t mHeight;
    uint32_t mDebug;
    uint32_t mReset;

    XEffectListUI *mEffectListUI;
    std::vector<XFrameLayer *> mFrameLayers;
    std::unordered_map<int, std::vector<XEffectUI *>> mEffectUIs;
    XSaturation *mFilter;
    int mCurrentLayer = 0;
};


ENTRY_IMPLEMENT_MAIN(ExampleLayers, "layers", "layers xiuxiuxiu.");