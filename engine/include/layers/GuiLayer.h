#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "core/ILayer.h"
#include "core/Application.h"

namespace Calyx {

    class GuiLayer : public ILayer {

    public:
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& event) override;

        void Begin();
        void End();

        virtual String GetName() const override { return "GUI Layer"; };

        static void SetupImGuiTheme();

    private:
        ImGuiContext* m_context;

    };

}