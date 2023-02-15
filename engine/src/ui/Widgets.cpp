#include "ui/Widgets.h"

namespace Calyx {

    bool Widgets::s_isWindowActions = false;

    void Widgets::Text(const String& text, bool disabled) {
        if (disabled)
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetColorU32(ImGuiCol_TextDisabled));
        ImGui::Text("%s", text.c_str());
        if (disabled)
            ImGui::PopStyleColor();
    }

    void Widgets::TextWithInfo(const String& text, const String& tooltip) {
        Widgets::Text(text);
        ImGui::SameLine();
        Widgets::TextDisabled(" (?)");
        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            Widgets::Text(tooltip);
            ImGui::EndTooltip();
        }
    }

    void Widgets::TextDisabled(const String& text) {
        Widgets::Text(text, true);
    }

    void Widgets::SmallText(const String& text, bool disabled) {
        Widgets::PushFont(CX_FONT_REGULAR + CX_FONT_SIZE_SMALL);
        Widgets::Text(text, disabled);
        Widgets::PopFont();
    }

    void Widgets::PaddedText(const String& text, const vec4& padding, bool disabled) {
        ImVec2 sz = ImGui::CalcTextSize(text.c_str());
        ImVec2 cursor = ImGui::GetCursorPos();
        ImGui::InvisibleButton(("##" + text).c_str(), { sz.x + padding.x + padding.y, sz.y + padding.z + padding.w });
        ImVec2 finalCursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos({ cursor.x + padding.x, cursor.y + padding.z });
        Widgets::Text(text, disabled);
        ImGui::SetCursorPos(finalCursor);
    }

    void Widgets::PaddedText(const String& text, const vec2& padding, bool disabled) {
        Widgets::PaddedText(text, { padding.x, padding.x, padding.y, padding.y }, disabled);
    }

    bool Widgets::Button(const String& label, bool disabled) {
        Widgets::PushFrameStyle(true);
        if (disabled)
            ImGui::BeginDisabled();
        bool ret = ImGui::Button(label.c_str());
        if (disabled)
            ImGui::EndDisabled();
        Widgets::PopFrameStyle();
        return ret;
    }

    bool Widgets::SmallButton(const String& label, bool disabled) {
        Widgets::PushFont(CX_FONT_REGULAR + CX_FONT_SIZE_SMALL);
        bool ret = Widgets::Button(label, disabled);
        Widgets::PopFont();
        return ret;
    }

    bool Widgets::IconButton(const String& label, bool disabled) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { s_framePadding, s_framePadding });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 15.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        if (disabled)
            ImGui::BeginDisabled();
        bool ret = ImGui::Button(label.c_str());
        if (disabled)
            ImGui::EndDisabled();
        ImGui::PopStyleVar(3);
        return ret;
    }

    bool Widgets::Combo(const String& label, const List<String>& options, int& value, bool fullWidth) {
        String inputLabel = RenderInputLabel(label);
        List<const char*> optionList;
        optionList.reserve(options.size());

        float largestWidth = 0.0f;
        for (const auto& option: options) {
            optionList.push_back(option.c_str());
            float width = ImGui::CalcTextSize(option.c_str()).x;
            if (!fullWidth) {
                if (width > largestWidth)
                    largestWidth = width;
            }
        }

        String comboLabel = RenderInputLabel(label);
        ImGui::SetNextItemWidth(fullWidth ? -FLT_MIN : std::max(largestWidth + 2 * s_framePadding + 30.0f, 150.0f));
        Widgets::PushFrameStyle();
        bool ret = ImGui::Combo(comboLabel.c_str(), &value, optionList.data(), optionList.size());
        Widgets::PopFrameStyle();

        return ret;
    }

    bool Widgets::InputText(const String& label, String& value, int flags, const String& tooltip) {
        String inputLabel = RenderInputLabel(label, tooltip);
        Widgets::NextItemFullWidth();
        Widgets::PushFrameStyle();
        bool ret = ImGui::InputText(inputLabel.c_str(), &value, flags);
        Widgets::PopFrameStyle();
        return ret;
    }

    bool Widgets::InputTextWithHint(
        const String& label, const String& hint, String& value, int flags, const String& tooltip
    ) {
        String inputLabel = RenderInputLabel(label, tooltip);
        Widgets::NextItemFullWidth();
        Widgets::PushFrameStyle();
        bool ret = ImGui::InputTextWithHint(inputLabel.c_str(), hint.c_str(), &value, flags);
        Widgets::PopFrameStyle();
        return ret;
    }

    bool Widgets::InputTextWithAppend(
        const String& label, const String& append, String& value, int flags, const String& tooltip
    ) {
        String inputLabel = RenderInputLabel(label, tooltip);
        auto contentRegion = ImGui::GetContentRegionAvail();
        auto appendWidth = ImGui::CalcTextSize(append.c_str()).x + 4.0f * s_framePadding;
        Widgets::PushFrameStyle();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { -1.0f, s_itemSpacing });
        ImGui::SetNextItemWidth(contentRegion.x - appendWidth);
        bool ret = ImGui::InputText(inputLabel.c_str(), &value, flags);
        ImGui::SameLine();
        auto& colors = ImGui::GetStyle().Colors;
        ImGui::PushStyleColor(ImGuiCol_Button, colors[ImGuiCol_ButtonHovered]);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colors[ImGuiCol_ButtonHovered]);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, colors[ImGuiCol_ButtonHovered]);
        Widgets::Button(append);
        ImGui::PopStyleColor(3);
        ImGui::PopStyleVar();
        Widgets::PopFrameStyle();
        return ret;
    }

    bool Widgets::InputSearch(const String& label, const String& hint, String& value, int flags) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, s_frameBorderSize);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0, 0 });
        ImGui::BeginChildFrame(ImGui::GetID(label.c_str()), { 0.0f, Widgets::GetFrameHeight() });
        ImGui::PopStyleVar(2);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { s_framePadding, s_framePadding });
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { 0.0f, 0.0f });
        bool changed = false;
        if (ImGui::BeginTable(label.c_str(), 3)) {
            ImGui::TableSetupColumn("Icon", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Clear", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
            Widgets::PaddedText(ICON_MD_SEARCH, { s_framePadding, 0.0f, s_framePadding, s_framePadding });
            ImGui::PopStyleVar();
            ImGui::TableNextColumn();
            Widgets::NextItemFullWidth();
            changed |= ImGui::InputTextWithHint(label.c_str(), hint.c_str(), &value, flags);
            if (!value.empty()) {
                ImGui::TableNextColumn();
                if (ImGui::Button(ICON_MD_CLEAR)) {
                    value = "";
                    changed = true;
                }
            }
            ImGui::EndTable();
        }
        ImGui::PopStyleVar(3);
        ImGui::EndChildFrame();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
        ImGui::InvisibleButton("Padding", { 1.0f, s_itemSpacing - 4.0f });
        ImGui::PopStyleVar();

        return changed;
    }

    bool Widgets::InputFilePath(const String& label, FileSelectionContext& context, const String& tooltip) {
        auto inputLabel = RenderInputLabel(label, tooltip);

        bool changed = false;
        if (ImGui::BeginTable(inputLabel.c_str(), 2)) {
            ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Button", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableNextRow();

            ImGui::TableNextColumn();
            Widgets::NextItemFullWidth();
            String strValue = context.m_selection.First();
            if (Widgets::InputText("##Path", strValue)) {
                context.m_selection.Clear();
                if (!strValue.empty()) {
                    Path path = FileSystem::absolute(strValue);
                    context.m_selection.Add(path);
                    context.SetDialogDirectory(path.parent_path(), false);
                }
            }
            if (ImGui::IsItemDeactivatedAfterEdit() && !context.m_selection.IsEmpty()) {
                auto file = context.m_selection.First();
                context.m_selection.Clear();
                context.m_selection.Add(PathUtils::Normalize(PathUtils::FirstExistingFile(file)));
            }

            ImGui::TableNextColumn();
            if (Widgets::Button(ICON_MD_FOLDER)) {
                ImGui::OpenPopup(label.c_str());
                context.m_selection.Normalize();
                context.m_dialogSelection = context.m_selection;
                if (!context.m_dialogSelection.IsEmpty()) {
                    Path dir = PathUtils::FirstExistingFile(Path(context.m_dialogSelection.First()).parent_path());
                    context.SetDialogDirectory(dir, false);
                }
            }

            static Path path;
            changed = Widgets::FileDialog(label, context);

            ImGui::EndTable();
        }

        return changed;
    }

    bool Widgets::PathBreadCrumbs(Path& path) {
        bool changed = false, first = true;
        Path currentPath;
        Path absolutePath = PathUtils::Normalize(path);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { s_itemSpacing, s_itemSpacing });
        for (const auto& part: absolutePath) {
            currentPath /= part;
            if (!first) {
                ImGui::SameLine();
            }
            if (Widgets::Button(part.string())) {
                path = currentPath;
                changed = true;
            }
            first = false;
        }
        ImGui::PopStyleVar();
        return changed;
    }

    bool Widgets::FileDialog(const String& title, FileSelectionContext& context) {
        bool confirmed = false;
        ImGui::SetNextWindowSizeConstraints({ 800.0f, -1.0f }, { INFINITY, -1.0f });
        ImGui::SetNextWindowSize({ 900.0f, 500.0f });
        if (ImGui::BeginPopupModal(
            title.c_str(), nullptr,
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar
        )) {
            Widgets::PushFont(CX_FONT_REGULAR + CX_FONT_SIZE_SMALL);
            if (Widgets::Button(ICON_MD_HOME)) {
                context.m_dialogSelection.Clear();
                context.SetDialogDirectory(PathUtils::GetUserHomeDirectory());
            }
            ImGui::SameLine();
            if (context.m_showCrumbsInput) {
                bool editDone = Widgets::SmallButton(ICON_MD_CHECK);
                ImGui::SameLine();
                if (context.m_focusCrumbsInput) {
                    ImGui::SetKeyboardFocusHere();
                    context.m_focusCrumbsInput = false;
                }
                Widgets::InputText("##PathInput", context.m_breadCrumbsInput);
                editDone |= ImGui::IsItemDeactivatedAfterEdit();
                if (editDone) {
                    context.SetDialogDirectory(PathUtils::FirstExistingFile(context.m_breadCrumbsInput));
                    context.m_showCrumbsInput = false;
                }
            } else {
                if (Widgets::SmallButton(ICON_MD_EDIT)) {
                    context.m_breadCrumbsInput = context.m_directory;
                    context.m_showCrumbsInput = true;
                    context.m_focusCrumbsInput = true;
                }
                ImGui::SameLine();
                if (Widgets::PathBreadCrumbs(context.m_directory)) {
                    context.SetDialogDirectory(context.m_directory);
                }
            }
            Widgets::PopFont();

            if (Widgets::InputSearch("##Search", "Filter by name...", context.m_search)) {
                context.SortFiles();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, s_itemSpacing - 2.0f });
            if (ImGui::BeginTable(
                "File List", 3,
                ImGuiTableFlags_ScrollY |
                ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_PadOuterX | ImGuiTableFlags_Borders |
                ImGuiTableFlags_NoBordersInBodyUntilResize | ImGuiTableFlags_RowBg | ImGuiTableFlags_Sortable |
                ImGuiTableFlags_Resizable,
                { 0, -2 * Widgets::GetFrameHeightWithSpacing() }
            )) {
                ImGui::TableSetupColumn("File Name", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 100);
                ImGui::TableSetupColumn("Last Modified", ImGuiTableColumnFlags_WidthFixed, 200);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                context.m_sortSpecs = ImGui::TableGetSortSpecs();
                if (context.m_sortSpecs->SpecsDirty) {
                    context.SortFiles();
                    context.m_sortSpecs->SpecsDirty = false;
                }

                Optional<Path> targetDirectory;
                for (const auto* file: context.m_sortedFiles) {
                    ImGui::PushID(file->name.c_str());
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();

                    String icon = file->type == FileSystem::file_type::directory ? ICON_MD_FOLDER : ICON_MD_DESCRIPTION;
                    icon += " ";
                    ImGui::PushStyleColor(ImGuiCol_Header, { CX_RGBA_FROM_HEX(0xFF0277BD) });
                    bool selected = context.m_dialogSelection.Contains(file->path.lexically_normal().string());
                    if (selected) {
                        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { CX_RGBA_FROM_HEX(0xFF0277BD) });
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, { CX_RGBA_FROM_HEX(0x600277BD) });
                    }
                    if (ImGui::Selectable(
                        (icon + file->name).c_str(),
                        selected, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_DontClosePopups |
                                  ImGuiSelectableFlags_AllowDoubleClick
                    )) {
                        context.m_dialogSelection.Clear();
                        context.m_dialogSelection.Add(file->path);
                    }
                    ImGui::PopStyleColor(2);

                    if (file->type == FileSystem::file_type::directory &&
                        ImGui::IsItemHovered() &&
                        ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                        targetDirectory = file->path;
                    }

                    ImGui::TableNextColumn();
                    if (file->size >= 0) {
                        Widgets::Text(FormatUtils::ByteSize(file->size));
                    } else {
                        Widgets::TextDisabled("--");
                    }

                    ImGui::TableNextColumn();
                    Widgets::Text(fmt::format("{:%m/%d/%y %I:%M %p}", file->modified));

                    ImGui::PopID();
                }

                if (targetDirectory.has_value()) {
                    context.SetDialogDirectory(targetDirectory.value());
                }

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();

            if (ImGui::BeginTable("File Name", 4)) {
                ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Input", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("New Folder", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("Filter", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                Widgets::PaddedText("Name", { 0, s_framePadding - 2.0f });
                ImGui::TableNextColumn();
                String name = context.m_dialogSelection.First(true);
                if (Widgets::InputText("##FileName", name)) {
                    context.m_dialogSelection.Clear();
                    if (!name.empty()) {
                        context.m_dialogSelection.Add(PathUtils::Normalize(context.m_directory / name));
                    }
                }
                if (ImGui::IsItemDeactivatedAfterEdit()) {
                    confirmed = true;
                }
                ImGui::TableNextColumn();
                if (Widgets::Button(
                    ICON_MD_CREATE_NEW_FOLDER,
                    context.m_dialogSelection.Size() != 1 || FileSystem::exists(context.m_dialogSelection.First()))) {
                    auto folder = context.m_dialogSelection.First();
                    FileSystem::create_directories(folder);
                    context.SetDialogDirectory(context.m_directory);
                    context.m_dialogSelection.Add(folder);
                }
                ImGui::TableNextColumn();
                if (Widgets::Combo("##Filters", context.m_options.filters, context.m_currentFilter)) {
                    context.m_dialogSelection.Clear();
                    context.SortFiles();
                }
                ImGui::EndTable();
            }

            if (Widgets::BeginActions("Actions", 2)) {
                if (Widgets::Button(
                    "Open", !context.m_dialogSelection.IsOK() &&
                            (!context.m_dialogSelection.IsEmpty() || !context.m_options.allowDirectorySelections))) {
                    confirmed = true;
                }
                Widgets::NextAction();
                if (Widgets::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }
                Widgets::EndActions();
            }

            if (confirmed) {
                if (context.m_dialogSelection.IsEmpty() && context.m_options.allowDirectorySelections) {
                    context.m_dialogSelection.Clear();
                    context.m_dialogSelection.Add(PathUtils::Normalize(context.m_directory));
                }

                if (context.m_dialogSelection.IsOK()) {
                    context.m_selection = context.m_dialogSelection;
                    ImGui::CloseCurrentPopup();
                } else {
                    confirmed = false;
                }
            }

            ImGui::EndPopup();
        }

        return confirmed;
    }

    void Widgets::InfoDialog(const String& title, const String& message, float width) {
        ImGui::SetNextWindowSizeConstraints({ width, -1.0f }, { width, -1.0f });
        if (ImGui::BeginPopupModal(
            title.c_str(), nullptr, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize
        )) {
            ImGui::TextWrapped("%s", message.c_str());
            Widgets::BeginActions("InfoActions", 1);
            if (Widgets::Button("OK")) {
                ImGui::CloseCurrentPopup();
            }
            Widgets::EndActions();
            ImGui::EndPopup();
        }
    }

    bool Widgets::BeginActions(const String& name, int num) {
        s_isWindowActions = true;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
        bool ret = ImGui::BeginTable(name.c_str(), num + 1);
        if (ret) {
            ImGui::TableSetupColumn("Blank", ImGuiTableColumnFlags_WidthStretch);
            for (int i = 0; i < num; i++) {
                ImGui::TableSetupColumn(("Action" + std::to_string(i)).c_str(), ImGuiTableColumnFlags_WidthFixed);
            }
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::InvisibleButton("Padding", { 1.0f, 1.0f });
            ImGui::TableNextColumn();
        }
        return ret;
    }

    void Widgets::NextAction() {
        ImGui::TableNextColumn();
    }

    void Widgets::EndActions() {
        ImGui::EndTable();
        ImGui::PopStyleVar();
        s_isWindowActions = false;
    }

    void Widgets::PushFrameStyle(bool button) {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, s_frameBorderSize);
        ImGui::PushStyleVar(
            ImGuiStyleVar_FramePadding, { button ? 2 * s_framePadding : s_framePadding, s_framePadding }
        );
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, s_isWindowActions ? 0.0f : s_itemSpacing });
    }

    void Widgets::PopFrameStyle() {
        ImGui::PopStyleVar(3);
    }

    void Widgets::PushFont(int font) {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[font]);
    }

    void Widgets::PopFont() {
        ImGui::PopFont();
    }

    void Widgets::NextItemFullWidth() {
        ImGui::SetNextItemWidth(-FLT_MIN);
    }

    String Widgets::RenderInputLabel(const String& label, const String& tooltip) {
        String inputLabel = label;
        if (!label.starts_with("##")) {
            inputLabel = "##" + inputLabel;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 7.0f });
            if (tooltip.empty()) {
                Text(label);
            } else {
                TextWithInfo(label, tooltip);
            }
            ImGui::PopStyleVar();
        }
        return inputLabel;
    }

    float Widgets::GetFrameHeight() {
        PushFrameStyle();
        const float result = ImGui::GetFrameHeight();
        PopFrameStyle();
        return result;
    }

    float Widgets::GetFrameHeightWithSpacing() {
        return Widgets::GetFrameHeight() + s_itemSpacing + 1.0f;
    }

    vec2 Widgets::GetIconButtonSize(const String& buttonLabel) {
        auto textSize = ImGui::CalcTextSize(buttonLabel.c_str());
        return vec2(textSize.x, textSize.y) + vec2(s_framePadding, s_framePadding);
    }

}