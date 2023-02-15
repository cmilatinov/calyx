#pragma once

#include "inspector/TypeInspectorFunctions.h"
#include "extra/TypeProcessor.h"

namespace Calyx::Editor {

    class ITypeInspector : public ITypeProcessor {
        CX_BEGIN_REFLECT();
    };

    template<typename T>
    class TypeInspector : public TypeProcessor<ITypeInspector, T> {};

}