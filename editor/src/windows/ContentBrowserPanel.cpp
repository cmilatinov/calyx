#include "windows/ContentBrowserPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Calyx::Editor {

    ContentBrowserPanel::ContentBrowserPanel(const String& basePath)
        : m_objFileTexture(AssetRegistry::LoadAsset<Texture2D>("assets/icons/block.png")),
          m_folderTexture(AssetRegistry::LoadAsset<Texture2D>("assets/icons/folder-grey.png")) {
        SetDirectory(std::filesystem::absolute(basePath));
    }

    void ContentBrowserPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { m_padding, m_padding });

        ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("<<")) {
                SetDirectory(m_currentDirectory.parent_path());
            }
            ImGui::EndMenuBar();
        }

        float lineHeight = ImGui::GetTextLineHeight();
        float cellSize = m_thumbnailSize + 2 * m_padding;
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        if (ImGui::BeginTable("Files", std::max(1, (int) (contentRegionWidth / cellSize)), 0)) {
            for (const auto& entry: std::filesystem::directory_iterator(m_currentDirectory)) {
                ImGui::TableNextColumn();
                auto& path = entry.path();
                String filename = path.filename().string();

                ImGui::PushStyleColor(ImGuiCol_Button, 0);
                ImGui::PushID(filename.c_str());
                auto textureID = entry.is_directory() ?
                                 reinterpret_cast<void*>(m_folderTexture->GetRendererID()) :
                                 reinterpret_cast<void*>(m_objFileTexture->GetRendererID());
                if (ImGui::ImageButton(textureID, { m_thumbnailSize, m_thumbnailSize }) && entry.is_directory()) {
                    SetDirectory(entry.path());
                }
                ImGui::PopID();
                ImGui::PopStyleColor();

                auto contentRegionX = ImGui::GetContentRegionAvail().x;
                auto textSize = ImGui::CalcTextSize(filename.c_str());
                auto textSizeX = std::min(textSize.x, m_thumbnailSize);
                auto* window = ImGui::GetCurrentWindow();
                auto cursorPos = window->DC.CursorPos;
                float textPosX = cursorPos.x + (m_padding + m_thumbnailSize - textSizeX) * 0.5f;
                float textPosMaxX = cursorPos.x + contentRegionX;
                ImGui::RenderTextEllipsis(
                    window->DrawList,
                    { textPosX, cursorPos.y },
                    { textPosMaxX, cursorPos.y + lineHeight },
                    textPosMaxX, textPosMaxX,
                    filename.c_str(), nullptr, nullptr
                );
                ImGui::Dummy({ textSizeX, textSize.y });

            }
            ImGui::EndTable();
        }
        ImGui::PopFont();

        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    void ContentBrowserPanel::SetDirectory(const Path& path) {
        m_currentDirectory = path;
        m_currentRelativePath = std::filesystem::relative(path, Path("."));
        CX_CORE_TRACE("{}", m_currentRelativePath.string());
    }

}