#pragma once

static const char* TEMPLATE_COMPONENT_HEADER =
    R"(#pragma once

#include "ecs/Component.h"

namespace {{ namespace }} {

    class {{ name }} : public Component<{{ name }}> {
        CX_BEGIN_REFLECT();

    };

})";