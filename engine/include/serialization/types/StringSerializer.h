#pragma once

#include "serialization/TypeSerializer.h"

namespace Calyx {

    class StringSerializer : public TypeSerializer<String> {
        CX_BEGIN_REFLECT();
        void CX_ON_SERIALIZE(std::ostream* stream, const String& value);
        void CX_ON_DESERIALIZE(std::istream* stream, String& value);
    };

}