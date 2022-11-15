#include "windows/ContentBrowserPanel.h"

namespace Calyx::Editor {

    ContentBrowserPanel::ContentBrowserPanel(const String& basePath)
        : m_currentDirectory(basePath) {}

    void ContentBrowserPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { m_padding, m_padding });
        ImGui::SetNextWindowSize({ 400, 400 });
        ImGui::Begin("Content Browser");

        float cellSize = m_thumbnailSize + 2 * m_padding;
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        if (ImGui::BeginTable("Files", std::max(1, (int) (contentRegionWidth / cellSize)), ImGuiTableFlags_PadOuterX)) {
            for (const auto& entry: std::filesystem::directory_iterator(m_currentDirectory)) {
                ImGui::TableNextColumn();
                String filename = entry.path().filename().string();
                ImGui::Button(filename.c_str(), { cellSize, cellSize });
                ImGui::Text(filename.c_str());
            }
            ImGui::EndTable();
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
    }

}