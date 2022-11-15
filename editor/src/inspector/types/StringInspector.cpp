#include "inspector/types/StringInspector.h"

namespace Calyx::Editor {

    void StringInspector::CX_ON_INSPECTOR_GUI(String& value) {
        InspectorGUI::TextControl("##text", value);
    }

}