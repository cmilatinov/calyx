#pragma once

#include "serialization/Serializer.h"

#define CX_DEFAULT_TYPE_SERIALIZER(type)                                    \
class DefaultSerializer_##type : public TypeSerializer<type> {              \
    CX_BEGIN_REFLECT();                                                     \
    void CX_ON_SERIALIZE(std::ostream* stream, const type& value);          \
    void CX_ON_DESERIALIZE(std::istream* stream, type& value);              \
}

#define CX_DEFAULT_TYPE_SERIALIZER_IMPL(type)                                               \
void DefaultSerializer_##type::CX_ON_SERIALIZE(std::ostream* stream, const type& value) {   \
    Serializer::DefaultSerialize(*stream, value);                                           \
}                                                                                           \
                                                                                            \
void DefaultSerializer_##type::CX_ON_DESERIALIZE(std::istream* stream, type& value) {       \
    Serializer::DefaultDeserialize(*stream, value);                                         \
}

namespace Calyx {

    CX_DEFAULT_TYPE_SERIALIZER(vec2);
    CX_DEFAULT_TYPE_SERIALIZER(vec3);
    CX_DEFAULT_TYPE_SERIALIZER(vec4);

    CX_DEFAULT_TYPE_SERIALIZER(mat2);
    CX_DEFAULT_TYPE_SERIALIZER(mat3);
    CX_DEFAULT_TYPE_SERIALIZER(mat4);

}