#pragma once

#include "inspector/SelectionManager.h"
#include "inspector/TypeInspector.h"
#include "inspector/InspectorGUI.h"
#include "ecs/Component.h"
#include "extra/TypeTracker.h"

namespace Calyx::Editor {

    class Inspector : public TypeTracker<ITypeInspector> {
    CX_SINGLETON(Inspector);

    public:
        Inspector();

        static void DrawComponentInspector(GameObject* gameObject, const entt::meta_type& type, void* instance) {
            auto ref = Reflect::Core::CreateOpaqueReference(type, instance);
            s_instance->_DrawComponentInspector(gameObject, ref);
        }

        template<typename T>
        static void DrawComponentInspector(GameObject* gameObject, T* instance) {
            auto ref = Reflect::Core::CreateReference(instance);
            s_instance->_DrawComponentInspector(gameObject, ref);
        }

    private:
        static String GetName(const entt::meta_any& instance);


    protected:
        void RegisterRefTypeProcessor(const entt::meta_type& type, const entt::meta_any& instance) override;


    private:
        Map<entt::id_type, entt::meta_any> m_inspectorClasses{};

        void _DrawComponentInspector(GameObject* gameObject, entt::meta_any& instance);
        void DrawPropertyInspector(const String& propertyName, entt::meta_any& instance);

        void DrawTypeInspector(entt::meta_any& inspector, entt::meta_any& instance);

        void DrawDefaultComponentInspector(entt::meta_any& component);

        bool DrawComponentContextInspector(GameObject* gameObject, entt::meta_any& component);
        bool DrawComponentInspectorHeader(GameObject* gameObject, entt::meta_any& instance);

        void InvokeInspectorFunction(
            const entt::meta_func& fn,
            entt::meta_any& inspector,
            entt::meta_any& instance
        );

    };

}