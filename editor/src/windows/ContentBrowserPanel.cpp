#include "windows/ContentBrowserPanel.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace Calyx::Editor {

    ContentBrowserPanel::ContentBrowserPanel(const Path& basePath)
        : m_objFileTexture(AssetRegistry::LoadAsset<Texture2D>("icons/block.png")),
          m_folderTexture(AssetRegistry::LoadAsset<Texture2D>("icons/folder-grey.png")),
          m_rootDirectory(std::filesystem::absolute(basePath)),
          m_currentDirectory(m_rootDirectory) {}

    ContentBrowserPanel::ContentBrowserPanel(const String& basePath)
        : m_objFileTexture(AssetRegistry::LoadAsset<Texture2D>("icons/block.png")),
          m_folderTexture(AssetRegistry::LoadAsset<Texture2D>("icons/folder-grey.png")),
          m_rootDirectory(std::filesystem::absolute(basePath)),
          m_currentDirectory(m_rootDirectory) {}

    void ContentBrowserPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8, 0 });
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { m_padding, m_padding });

        ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("<<")) {
                SetDirectory(m_currentDirectory.parent_path());
            }

            // Dynamically create the breadcrumbs
            Path tempPath = m_rootDirectory;
            ImGui::Text("/");
            for (auto& file: std::filesystem::relative(m_currentDirectory, m_rootDirectory)) {
                if (file.string() == ".")
                    continue;

                tempPath /= file;
                if (ImGui::MenuItem(file.string().c_str())) {
                    SetDirectory(tempPath);
                    break;
                }
                ImGui::Text("/");
            }

            ImGui::EndMenuBar();
        }

        float lineHeight = ImGui::GetTextLineHeight();
        float cellSize = m_thumbnailSize + 2 * m_padding;
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[CX_FONT_REGULAR + CX_FONT_SIZE_SMALL]);
        if (ImGui::BeginTable("Files", std::max(1, (int)(contentRegionWidth / cellSize)), 0)) {
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
        auto relativePath = FileSystem::relative(path, m_rootDirectory).string();
        if (relativePath.starts_with(".."))
            return;

        m_currentDirectory = path;
    }

}