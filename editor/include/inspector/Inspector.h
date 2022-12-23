#pragma once

#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"
#include "ecs/Component.h"

namespace Calyx::Editor {

    class Inspector {
    CX_SINGLETON(Inspector);

    public:
        Inspector();

        static void DrawComponentInspector(const entt::meta_type& type, void* instance) {
            auto ref = Reflect::Core::CreateOpaqueReference(type, instance);
            s_instance->_DrawComponentInspector(ref);
        }

        template<typename T>
        static void DrawComponentInspector(T* instance) {
            auto ref = Reflect::Core::CreateReference(instance);
            s_instance->_DrawComponentInspector(ref);
        }

    private:
        static String GetName(const entt::meta_any& instance);

    private:
        Map<entt::id_type, entt::meta_any> m_inspectorClasses{};

        void _DrawComponentInspector(entt::meta_any& instance);
        void DrawPropertyInspector(const String& propertyName, entt::meta_any& instance);

        void DrawTypeInspector(entt::id_type typeId, entt::meta_any& instance);

        void DrawDefaultComponentInspector(entt::meta_any& component);

        void DrawComponentContextInspector(entt::meta_any& component);
        bool DrawComponentInspectorHeader(entt::meta_any& instance);

        bool CheckInspectorTypeExists(const entt::meta_any& instance, entt::id_type* typeId = nullptr);
        bool CheckInspectorFunctionExists(
            entt::id_type function,
            const entt::meta_any& inspector,
            entt::meta_func* fn = nullptr
        );

        void InvokeInspectorFunction(
            const entt::meta_func& fn,
            entt::meta_any& inspector,
            entt::meta_any& instance
        );

    };

}