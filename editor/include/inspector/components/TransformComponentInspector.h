#pragma once

#include "ecs/components/TransformComponent.h"
#include "inspector/Inspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class TransformComponentInspector : public TypeInspector<TransformComponent> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_CONTEXT_GUI(IdentifierT typeID, TransformComponent& tc);
        void CX_ON_INSPECTOR_GUI(IdentifierT typeID, TransformComponent& tc);
    };

}