#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class FloatInspector : public TypeInspector<float> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(float& value);
    };

}