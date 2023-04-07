#pragma once

#include "assets/Mesh.h"
#include "render/CameraEditor.h"
#include "scene/SceneRenderer.h"
#include "inspector/SelectionManager.h"

#include <imgui.h>
#include <ImGuizmo.h>

namespace Calyx::Editor {

    struct ViewportState {
        vec2 size;
        vec4 bounds;
        bool pressed;
        bool hovered;
        bool focused;
    };

    class ViewportPanel {

    public:
        ViewportPanel();

        void OnUpdate();
        void OnGUI();
        void OnEvent(Event& event);

    private:
        void Toolbar();
        void SceneViewport();
        void UpdateViewport();
        void Gizmos();
        void RestoreSelection(const Selection& selection, const Scene& targetScene);

    private:
        Scope<SceneRenderer> m_sceneRenderer;
        CameraEditor m_editorCamera;

        // Framebuffers
        Ref<Framebuffer> m_framebuffer;
        Ref<Framebuffer> m_msaaFramebuffer;

        // Viewport state
        bool m_focusNextFrame = false;
        ViewportState m_viewport{};

        // Gizmos
        int m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;

    };

}