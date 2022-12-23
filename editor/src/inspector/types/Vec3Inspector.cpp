#include "inspector/types/Vec3Inspector.h"

namespace Calyx::Editor {

    void Vec3Inspector::CX_ON_INSPECTOR_GUI(IdentifierT typeID, vec3& value) {
        InspectorGUI::Vec3Control("##vec3", value);
    }

}