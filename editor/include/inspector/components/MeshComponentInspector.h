#pragma once

#include "inspector/TypeInspector.h"
#include "ecs/components/MeshComponent.h"

namespace Calyx::Editor {

    class MeshComponentInspector : public TypeInspector<MeshComponent> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(MeshComponent& mc);
    };

}