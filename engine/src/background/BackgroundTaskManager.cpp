#include "background/BackgroundTaskManager.h"
#include "ui/Widgets.h"

#include <imgui.h>

namespace Calyx::Editor {

    CX_SINGLETON_INSTANCE(BackgroundTaskManager);

    void BackgroundTaskManager::Draw() {
        if (m_openPopupNextFrame) {
            m_openPopupNextFrame = false;
            ImGui::OpenPopup("Waiting for tasks");
        }
        if (Widgets::BeginPopupModal("Waiting for tasks", 300.0f)) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 10 });
            Widgets::Text("Building assemblies ...");
            Widgets::PushFrameStyle();
            ImGui::PushStyleColor(ImGuiCol_PlotHistogram, CX_COLOR_PRIMARY);
            ImGui::ProgressBar(m_progress, { -FLT_MIN, 20 }, "");
            ImGui::PopStyleColor();
            Widgets::PopFrameStyle();
            ImGui::PopStyleVar();
            m_progress = std::fmod(m_progress + 0.003f, 1.0f);
            if (std::abs(m_progress - 1.0f) < 0.01) {
                ImGui::CloseCurrentPopup();
            }
            Widgets::EndPopup();
        }
    }

    void BackgroundTaskManager::BuildAssemblies() {
        m_openPopupNextFrame = true;
    }

}