#include "inspector/types/Vec4Inspector.h"

namespace Calyx::Editor {

    void Vec4Inspector::CX_ON_INSPECTOR_GUI(vec4& value) {
        InspectorGUI::ColorControl("##color", value);
    }

}