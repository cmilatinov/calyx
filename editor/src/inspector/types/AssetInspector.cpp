#include "inspector/types/AssetInspector.h"
#include "assets/Mesh.h"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace Calyx::Editor {

    void AssetInspector::CX_ON_INSPECTOR_GUI(IdentifierType typeID, Ref<IAsset>& value) {
        InspectorGUI::GameAssetControl("Asset", typeID, value);
    }

}