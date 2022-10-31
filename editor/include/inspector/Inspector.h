#pragma once

#include <imgui.h>

#include "inspector/TypeInspector.h"
#include "ecs/Component.h"

namespace Calyx::Editor {

    class Inspector {
        using InspectorMap = std::unordered_map<entt::id_type, entt::meta_any>;

    public:
        static void Init();

        static void DrawComponentInspector(const entt::meta_type& type, void* instance) {
            DrawComponentInspector(type.from_void(instance));
        }

        template<typename T>
        static void DrawComponentInspector(T* instance) {
            DrawComponentInspector(Reflect::Core::CreateReference(instance));
        }

        static void DrawPropertyInspector(const entt::meta_type& type, void* instance) {
            DrawPropertyInspector(type.from_void(instance));
        }

        template<typename T>
        static void DrawPropertyInspector(T* instance) {
            DrawPropertyInspector(Reflect::Core::CreateReference(instance));
        }

    private:
        static InspectorMap s_inspectorClasses;

        static String GetName(const entt::meta_any& instance);

        static void DrawComponentInspector(entt::meta_any&& instance);
        static void DrawPropertyInspector(entt::meta_any&& instance, const char* label = nullptr);

        static void DrawDefaultComponentInspector(entt::meta_any&& component);

        static void DrawComponentContextInspector(entt::meta_any&& component);
        static bool DrawComponentInspectorHeader(entt::meta_any&& instance);

        static bool CheckInspectorTypeExists(const entt::meta_any& instance, entt::id_type* typeId = nullptr);
        static bool CheckInspectorFunctionExists(
            entt::id_type function,
            const entt::meta_any& inspector,
            entt::meta_func* fn = nullptr
        );

    };

}