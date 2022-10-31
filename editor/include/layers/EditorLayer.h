#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>
#include <ImGuizmo.h>
#include <Calyx.h>

#include "math/ScreenSpaceUtils.h"
#include "input/Input.h"
#include "render/CameraEditor.h"
#include "scene/SceneRenderer.h"
#include "windows/SceneHierarchyPanel.h"
#include "inspector/Inspector.h"

namespace Calyx::Editor {

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

        // Grid
        Mesh* m_grid;
        Shader* m_gridShader;

        // Framebuffers
        Ref<Framebuffer> m_framebuffer;
        Ref<Framebuffer> m_msaaFramebuffer;

        // Viewport
        bool m_viewportPressed;
        bool m_viewportHovered;
        ImVec2 m_viewportSize;
        vec4 m_viewportBounds;

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