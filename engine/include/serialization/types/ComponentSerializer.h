#pragma once

#include "ecs/Component.h"
#include "serialization/TypeSerializer.h"

namespace Calyx {

    class ComponentSerializer : public TypeSerializer<IComponent*> {
        CX_BEGIN_REFLECT();
        void CX_ON_SERIALIZE(std::ostream* stream, const IComponent*& value);
        void CX_ON_DESERIALIZE(std::istream* stream, IComponent*& value);
    };

}