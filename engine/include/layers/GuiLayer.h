#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>
#include <implot.h>

#include "core/Layer.h"
#include "core/Application.h"
#include "ui/Definitions.h"

namespace Calyx {

    class CALYX_API GuiLayer : public ILayer {

    public:
        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

        String GetName() const override { return "GUI Layer"; };

    private:
        static void SetupImGuiTheme();

    private:
        ImGuiContext* m_context;

    };

}