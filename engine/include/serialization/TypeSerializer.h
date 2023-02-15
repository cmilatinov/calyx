#pragma once

#include "serialization/TypeSerializerFunctions.h"
#include "extra/TypeProcessor.h"

namespace Calyx {

    class ITypeSerializer : public ITypeProcessor {
        CX_BEGIN_REFLECT();
    };

    template<typename T>
    class TypeSerializer : public TypeProcessor<ITypeSerializer, T> {};

}