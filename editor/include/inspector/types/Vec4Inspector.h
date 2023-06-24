#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class Vec4Inspector : public TypeInspector<vec4> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(vec4& value);
    };

}