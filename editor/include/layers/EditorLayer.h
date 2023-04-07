#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>
#include <ImGuizmo.h>
#include <implot.h>
#include <efsw/efsw.hpp>

#include "math/ScreenSpaceUtils.h"
#include "input/Input.h"
#include "inspector/Inspector.h"
#include "inspector/InspectorGUI.h"
#include "assets/Assets.h"

#include "windows/SceneHierarchyPanel.h"
#include "windows/ContentBrowserPanel.h"
#include "windows/StatisticsPanel.h"
#include "windows/ViewportPanel.h"
#include "windows/InspectorPanel.h"

namespace Calyx::Editor {

    class EditorLayer : public ILayer {

    public:
        ~EditorLayer() override;

        void OnAttach() override;
        void OnUpdate() override;
        void OnGUI() override;
        void OnEvent(Event& event) override;

        String GetName() const override { return "Editor Layer"; }

    private:
        // Scene
        SceneHierarchyPanel m_sceneHierarchyPanel;
        ContentBrowserPanel m_contentBrowserPanel;
        StatisticsPanel m_statsPanel;
        ViewportPanel m_viewportPanel;
        InspectorPanel m_inspectorPanel;

        void BeginDockspace();
        void MenuBar();
        void EndDockspace();

    };

}