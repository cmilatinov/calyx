#include "inspector/types/AssetInspector.h"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace Calyx::Editor {

    void AssetInspector::CX_ON_INSPECTOR_GUI(Ref<IAsset>& value) {
        String str = std::to_string(value->GetAssetType());
        ImGui::InputText("##Name", &str, ImGuiInputTextFlags_ReadOnly);
        value = Ref<IAsset>();
    }

}