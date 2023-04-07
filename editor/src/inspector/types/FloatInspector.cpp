#include "inspector/types/FloatInspector.h"

namespace Calyx::Editor {

    void FloatInspector::CX_ON_INSPECTOR_GUI(float& value) {
        InspectorGUI::FloatControl("##value", value);
    }

}