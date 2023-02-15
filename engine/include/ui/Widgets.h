#pragma once

#include <imgui.h>
#include <imgui_stdlib.h>
#include <IconsMaterialDesign.h>

#include "ui/Definitions.h"
#include "ui/FileDialog.h"

namespace Calyx {

    class Widgets {
    CX_SINGLETON(Widgets);

    public:
        Widgets() = default;

        static void Text(const String& text, bool disabled = false);
        static void TextWithInfo(const String& text, const String& tooltip);
        static void TextDisabled(const String& text);
        static void SmallText(const String& text, bool disabled = false);
        static void PaddedText(const String& text, const vec4& padding, bool disabled = false);
        static void PaddedText(const String& text, const vec2& padding, bool disabled = false);
        static bool Button(const String& label, bool disabled = false);
        static bool SmallButton(const String& label, bool disabled = false);
        static bool IconButton(const String& label, bool disabled = false);
        static bool Combo(const String& label, const List<String>& options, int& value, bool fullWidth = false);
        static bool InputText(const String& label, String& value, int flags = ImGuiInputTextFlags_None, const String& tooltip = "");
        static bool InputTextWithHint(const String& label, const String& hint, String& value, int flags = ImGuiInputTextFlags_None, const String& tooltip = "");
        static bool InputTextWithAppend(const String& label, const String& append, String& value, int flags = ImGuiInputTextFlags_None, const String& tooltip = "");
        static bool InputSearch(const String& label, const String& hint, String& value, int flags = ImGuiInputTextFlags_None);
        static bool InputFilePath(const String& label, FileSelectionContext& context, const String& tooltip = "");
        static bool PathBreadCrumbs(Path& path);
        static bool FileDialog(const String& title, FileSelectionContext& context);
        static void InfoDialog(const String& title, const String& message, float width = 450.0f);

        static bool BeginActions(const String& name, int num);
        static void NextAction();
        static void EndActions();

        static constexpr float GetItemSpacing() { return s_itemSpacing; }
        static float GetFrameHeight();
        static float GetFrameHeightWithSpacing();
        static vec2 GetIconButtonSize(const String& buttonLabel);

        static void PushFont(int font);
        static void PopFont();

    private:
        static void PushFrameStyle(bool button = false);
        static void PopFrameStyle();
        static void NextItemFullWidth();

        static String RenderInputLabel(const String& label, const String& tooltip = "");

    private:
        static constexpr float s_itemSpacing = 8.0f;
        static constexpr float s_framePadding = 5.0f;
        static constexpr float s_frameBorderSize = 1.0f;
        static bool s_isWindowActions;

    };

}