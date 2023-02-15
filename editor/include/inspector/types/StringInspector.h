#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"

namespace Calyx::Editor {

    class StringInspector : public TypeInspector<String> {
        CX_BEGIN_REFLECT();
        void CX_ON_INSPECTOR_GUI(String& value);
    };

}