#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class AssetInspector : public TypeInspector<Ref<IAsset>> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(Ref<IAsset>& value);
    };

}