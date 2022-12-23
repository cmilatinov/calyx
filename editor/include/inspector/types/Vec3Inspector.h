#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class Vec3Inspector : public TypeInspector<vec3> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(IdentifierT typeID, vec3& value);
    };

}