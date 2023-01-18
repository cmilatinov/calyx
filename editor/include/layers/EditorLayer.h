#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>
#include <ImGuizmo.h>
#include <implot.h>
#include <efsw/efsw.hpp>

#include "math/ScreenSpaceUtils.h"
#include "input/Input.h"
#include "render/CameraEditor.h"
#include "scene/SceneRenderer.h"
#include "windows/SceneHierarchyPanel.h"
#include "windows/ContentBrowserPanel.h"
#include "windows/StatisticsPanel.h"
#include "inspector/Inspector.h"
#include "inspector/InspectorGUI.h"
#include "assets/Assets.h"

namespace Calyx::Editor {

    struct ViewportState {
        vec2 size;
        vec4 bounds;
        bool pressed;
        bool hovered;
    };

    class EditorLayer : public ILayer {

    public:
        void OnAttach() override;
        void OnUpdate() override;
        void OnGUI() override;
        void OnEvent(Event& event) override;

        String GetName() const override { return "Editor Layer"; };

    private:
        // Scene
        Scope<SceneRenderer> m_sceneRenderer;
        Scope<Scene> m_scene;
        Scope<CameraEditor> m_editorCamera;
        Scope<SceneHierarchyPanel> m_sceneHierarchyPanel;
        Scope<ContentBrowserPanel> m_contentBrowserPanel;
        Scope<StatisticsPanel> m_statsPanel;

        // Grid
        Ref<Mesh> m_grid;
        Ref<Shader> m_gridShader;

        // Framebuffers
        Ref<Framebuffer> m_framebuffer;
        Ref<Framebuffer> m_msaaFramebuffer;

        // Viewport
        ViewportState m_viewport{};

        // Gizmos
        int m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;

        // Components
        List<entt::meta_type> m_reflectedComponents;

        // TODO: Remove
        GameObject* m_cube;

        void BeginDockspace();
        void MenuBar();
        void Viewport();
        void SceneHierarchy();
        void Statistics();
        void Inspector();
        void ContentBrowser();
        void EndDockspace();

    };

}