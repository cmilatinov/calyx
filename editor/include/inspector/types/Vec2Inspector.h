#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class Vec2Inspector : public TypeInspector<vec2> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(IdentifierT typeID, vec2& value);
    };

}