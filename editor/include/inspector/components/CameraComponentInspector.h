#pragma once

#include "inspector/TypeInspector.h"
#include "ecs/components/CameraComponent.h"

namespace Calyx::Editor {

    class CameraComponentInspector : public TypeInspector<CameraComponent> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(CameraComponent& tc);
    };

}