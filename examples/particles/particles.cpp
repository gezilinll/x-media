/*
 * Copyright 2011-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include "common.h"
#include <entry/cmd.h>
#include <entry/input.h>

#include <camera.h>
#include <debugdraw/debugdraw.h>
#include <imgui/imgui.h>

#include <bx/rng.h>
#include <bx/easing.h>

#include "XParticles.hpp"
#include "XImageFileOutput.hpp"
#include "XImageUtils.hpp"

using namespace XImage;

namespace {
    XParticles *particles = new XParticles();
    XImageFileOutput *fileOutput = new XImageFileOutput();

    static const char *s_shapeNames[] =
            {
                    "Sphere",
                    "Hemisphere",
                    "Circle",
                    "Disc",
                    "Rect",
            };

    static const char *s_directionName[] =
            {
                    "Up",
                    "Outward",
            };

    static const char *s_easeFuncName[] =
            {
                    "Linear",
                    "Step",
                    "SmoothStep",
                    "InQuad",
                    "OutQuad",
                    "InOutQuad",
                    "OutInQuad",
                    "InCubic",
                    "OutCubic",
                    "InOutCubic",
                    "OutInCubic",
                    "InQuart",
                    "OutQuart",
                    "InOutQuart",
                    "OutInQuart",
                    "InQuint",
                    "OutQuint",
                    "InOutQuint",
                    "OutInQuint",
                    "InSine",
                    "OutSine",
                    "InOutSine",
                    "OutInSine",
                    "InExpo",
                    "OutExpo",
                    "InOutExpo",
                    "OutInExpo",
                    "InCirc",
                    "OutCirc",
                    "InOutCirc",
                    "OutInCirc",
                    "InElastic",
                    "OutElastic",
                    "InOutElastic",
                    "OutInElastic",
                    "InBack",
                    "OutBack",
                    "InOutBack",
                    "OutInBack",
                    "InBounce",
                    "OutBounce",
                    "InOutBounce",
                    "OutInBounce",
            };
    BX_STATIC_ASSERT(BX_COUNTOF(s_easeFuncName) == bx::Easing::Count);

    struct Emitter {
        EmitterUniforms m_uniforms;
        EmitterHandle m_handle;

        EmitterShape::Enum m_shape;
        EmitterDirection::Enum m_direction;

        void create() {
            m_shape = EmitterShape::Sphere;
            m_direction = EmitterDirection::Outward;

            m_handle = particles->createEmitter(m_shape, m_direction, 1024);
            m_uniforms.reset();
        }

        void destroy() {
            particles->destroyEmitter(m_handle);
        }

        void update() {
            particles->updateEmitter(m_handle, &m_uniforms);
        }

        void imgui() {
            if (ImGui::Combo("Shape", (int *) &m_shape, s_shapeNames, BX_COUNTOF(s_shapeNames))
                || ImGui::Combo("Direction", (int *) &m_direction, s_directionName, BX_COUNTOF(s_directionName))) {
                particles->destroyEmitter(m_handle);
                m_handle = particles->createEmitter(m_shape, m_direction, 1024);
            }

            ImGui::SliderInt("particles / s", (int *) &m_uniforms.particlesPerSecond, 0, 1024);
            ImGui::SliderFloat("Gravity scale", &m_uniforms.gravityScale, -2.0f, 2.0f);
            ImGui::RangeSliderFloat("Life span", &m_uniforms.lifeSpan[0], &m_uniforms.lifeSpan[1], 0.1f, 5.0f);

            if (ImGui::Button("Reset")) {
                m_uniforms.reset();
                particles->updateEmitter(m_handle, &m_uniforms);
            }

            if (ImGui::CollapsingHeader("Position and scale")) {
                ImGui::Combo("Position Ease", (int *) &m_uniforms.easePos, s_easeFuncName,
                             BX_COUNTOF(s_easeFuncName));
                ImGui::RangeSliderFloat("Start offset", &m_uniforms.offsetStart[0], &m_uniforms.offsetStart[1],
                                        0.0f, 10.0f);
                ImGui::RangeSliderFloat("End offset", &m_uniforms.offsetEnd[0], &m_uniforms.offsetEnd[1], 0.0f,
                                        10.0f);
                ImGui::Text("Scale:");
                ImGui::Combo("Scale Ease", (int *) &m_uniforms.easeScale, s_easeFuncName, BX_COUNTOF(s_easeFuncName));
                ImGui::RangeSliderFloat("Scale Start", &m_uniforms.scaleStart[0], &m_uniforms.scaleStart[1], 0.0f,
                                        3.0f);
                ImGui::RangeSliderFloat("Scale End", &m_uniforms.scaleEnd[0], &m_uniforms.scaleEnd[1], 0.0f, 3.0f);
            }

            if (ImGui::CollapsingHeader("Blending and color")) {
                ImGui::Combo("Blend Ease", (int *) &m_uniforms.easeBlend, s_easeFuncName, BX_COUNTOF(s_easeFuncName));
                ImGui::RangeSliderFloat("Blend Start", &m_uniforms.blendStart[0], &m_uniforms.blendStart[1], 0.0f,
                                        1.0f);
                ImGui::RangeSliderFloat("Blend End", &m_uniforms.blendEnd[0], &m_uniforms.blendEnd[1], 0.0f, 1.0f);
                ImGui::Text("Color:");
                ImGui::Combo("RGBA Ease", (int *) &m_uniforms.easeRgba, s_easeFuncName, BX_COUNTOF(s_easeFuncName));
                ImGui::ColorWheel("RGBA0", &m_uniforms.rgba[0], 0.3f);
                ImGui::ColorWheel("RGBA1", &m_uniforms.rgba[1], 0.3f);
                ImGui::ColorWheel("RGBA2", &m_uniforms.rgba[2], 0.3f);
                ImGui::ColorWheel("RGBA3", &m_uniforms.rgba[3], 0.3f);
                ImGui::ColorWheel("RGBA4", &m_uniforms.rgba[4], 0.3f);
            }
        }

        void gizmo(const float *_view, const float *_proj) {
            float mtx[16];
            float scale[3] = {1.0f, 1.0f, 1.0f};

            ImGuizmo::RecomposeMatrixFromComponents(m_uniforms.position, m_uniforms.angle, scale, mtx);

            ImGuiIO &io = ImGui::GetIO();
            ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

            ImGuizmo::Manipulate(_view, _proj, ImGuizmo::TRANSLATE, ImGuizmo::LOCAL, mtx);

            ImGuizmo::DecomposeMatrixToComponents(mtx, m_uniforms.position, m_uniforms.angle, scale);
        }
    };

    class ExampleParticles : public entry::AppI {
    public:
        ExampleParticles(const char *_name, const char *_description)
                : entry::AppI(_name, _description) {
        }

        void init(int32_t _argc, const char *const *_argv, uint32_t _width, uint32_t _height) override {
            Args args(_argc, _argv);

            m_width = _width;
            m_height = _height;
            m_debug = BGFX_DEBUG_NONE;
            m_reset = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type = args.m_type;
            init.vendorId = args.m_pciId;
            init.resolution.width = m_width;
            init.resolution.height = m_height;
            init.resolution.reset = m_reset;
            bgfx::init(init);

            // Enable m_debug text.
            bgfx::setDebug(m_debug);

            // Set view 0 clear state.
            bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x202020ff, 1.0f, 0);

            ddInit();

            particles->init();
            fileOutput->addTarget(particles);
            EmitterSpriteHandle sprite = particles->createSprite("textures/particle.ktx", bgfx::TextureFormat::BGRA8);
            for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii) {
                m_emitter[ii].create();
                m_emitter[ii].m_uniforms.sprite = sprite;
                m_emitter[ii].update();
            }

            imguiCreate();

            cameraCreate();

            cameraSetPosition({0.0f, 2.0f, -12.0f});
            cameraSetVerticalAngle(0.0f);

            m_timeOffset = bx::getHPCounter();
        }

        virtual int shutdown() override {
            for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii) {
                m_emitter[ii].destroy();
            }

            SAFE_DELETE(fileOutput);
            SAFE_DELETE(particles);

            ddShutdown();

            imguiDestroy();

            cameraDestroy();

            // Shutdown bgfx.
            bgfx::shutdown();

            return 0;
        }

        bool update() override {
            if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState)) {
                // Set view 0 default viewport.
                bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

                bgfx::touch(0);

                int64_t now = bx::getHPCounter() - m_timeOffset;
                static int64_t last = now;
                const int64_t frameTime = now - last;
                last = now;
                const double freq = double(bx::getHPFrequency());
                const float deltaTime = float(frameTime / freq);

                cameraUpdate(deltaTime, m_mouseState);

                float view[16];
                cameraGetViewMtx(view);

                float proj[16];

                // Set view and projection matrix for view 0.
                {
                    bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f,
                                bgfx::getCaps()->homogeneousDepth);

                    bgfx::setViewTransform(0, view, proj);
                    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
                }

                imguiBeginFrame(
                        m_mouseState.m_mx, m_mouseState.m_my,
                        (m_mouseState.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
                        | (m_mouseState.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
                        | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0),
                        m_mouseState.m_mz, uint16_t(m_width), uint16_t(m_height)
                );

                showExampleDialog(this);

                ImGui::SetNextWindowPos(
                        ImVec2(m_width - m_width / 4.0f - 10.0f, 10.0f), ImGuiCond_FirstUseEver);
                ImGui::SetNextWindowSize(
                        ImVec2(m_width / 4.0f, m_height - 20.0f), ImGuiCond_FirstUseEver);
                ImGui::Begin("Settings", NULL);

                static float timeScale = 1.0f;
                ImGui::SliderFloat("Time scale", &timeScale, 0.0f, 1.0f);

                static bool showBounds;
                ImGui::Checkbox("Show bounds", &showBounds);

                ImGui::Text("Emitter:");
                static int currentEmitter = 0;
                for (uint32_t ii = 0; ii < BX_COUNTOF(m_emitter); ++ii) {
                    ImGui::SameLine();

                    char name[16];
                    bx::snprintf(name, BX_COUNTOF(name), "%d", ii);

                    ImGui::RadioButton(name, &currentEmitter, ii);
                }

                m_emitter[currentEmitter].imgui();

                ImGui::End();

                m_emitter[currentEmitter].gizmo(view, proj);

                imguiEndFrame();

                DebugDrawEncoder dde;
                dde.begin(0);

                dde.drawGrid(Axis::Y, {0.0f, 0.0f, 0.0f});

                const bx::Vec3 eye = cameraGetPosition();

                m_emitter[currentEmitter].update();
                particles->updateViewAndEye(view, eye);
                fileOutput->render(deltaTime * timeScale);

                if (showBounds) {
                    Aabb aabb;
                    particles->getAabb(m_emitter[currentEmitter].m_handle, aabb);
                    dde.push();
                    dde.setWireframe(true);
                    dde.setColor(0xff0000ff);
                    dde.draw(aabb);
                    dde.pop();
                }

                dde.end();

                bgfx::frame();

                return true;
            }

            return false;
        }

        entry::MouseState m_mouseState;

        int64_t m_timeOffset;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;

        Emitter m_emitter[4];
    };
} // namespace

ENTRY_IMPLEMENT_MAIN(ExampleParticles, "Particles", "Particles.");
