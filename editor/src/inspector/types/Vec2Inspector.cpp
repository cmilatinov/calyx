#include "inspector/types/Vec2Inspector.h"

namespace Calyx::Editor {

    void Vec2Inspector::CX_ON_INSPECTOR_GUI(vec2& value) {
        InspectorGUI::Vec2Control("##vec2", value);
    }

}