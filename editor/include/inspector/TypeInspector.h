#pragma once

#include <imgui.h>

#include "inspector/TypeInspectorFunctions.h"

namespace Calyx::Editor {

    class ITypeInspector {
        CX_BEGIN_REFLECT();

    public:
        virtual ~ITypeInspector() = default;

        virtual entt::meta_type GetMetaType() const = 0;

    };

    template<typename T>
    class TypeInspector : public ITypeInspector {

    public:
        TypeInspector()
            : m_metaType(entt::resolve<T>()) {}

        entt::meta_type GetMetaType() const override { return m_metaType; }

        virtual void CX_ON_INSPECTOR_CONTEXT_GUI(T& component) {}
        virtual void CX_ON_INSPECTOR_GUI(T& component) {}

    private:
        entt::meta_type m_metaType;

    };

}