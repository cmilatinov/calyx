#include "inspector/Inspector.h"

#include <imgui.h>

namespace Calyx::Editor {

    Inspector::InspectorMap Inspector::s_inspectorClasses{};

    void Inspector::Init() {
        s_inspectorClasses.clear();
        List<entt::meta_type> inspectorClasses = Reflect::Core::GetDerivedClasses<ITypeInspector>();
        for (const auto& inspector: inspectorClasses) {
            auto instance = inspector.construct();
            if (!instance)
                continue;

            auto* typeInspector = instance.try_cast<ITypeInspector>();
            if (typeInspector == nullptr) {
                instance.reset();
                continue;
            }

            auto type = typeInspector->GetMetaType();
            if (Reflect::Core::IsRefType(type)) {
                auto ptrType = type.template_arg(0);
                s_inspectorClasses[ptrType.id()] = instance;
                List<entt::meta_type> derivedClasses = Reflect::Core::GetDerivedClasses(ptrType);
                for (const auto& derived: derivedClasses) {
                    s_inspectorClasses[derived.id()] = instance;
                }
            } else {
                s_inspectorClasses[type.id()] = instance;
            }
        }
    }

    String Inspector::GetName(const entt::meta_any& instance) {
        String name;
        const IComponent* component;
        if ((component = instance.try_cast<IComponent>()) != nullptr) {
            name = component->GetName();
        } else {
            name = String(instance.type().info().name());
        }
        // TODO: Check for custom component name via instance.type().prop()
        return name;
    }

    void Inspector::DrawComponentInspector(entt::meta_any&& instance) {
        if (!DrawComponentInspectorHeader(std::forward<entt::meta_any>(instance)))
            return;

        entt::id_type typeId;
        if (!CheckInspectorTypeExists(instance, &typeId))
            return DrawDefaultComponentInspector(std::forward<entt::meta_any>(instance));

        DrawTypeInspector(typeId, std::forward<entt::meta_any>(instance));
    }

    void Inspector::DrawPropertyInspector(const String& propertyName, entt::meta_any&& instance) {
        entt::id_type typeId;
        if (!CheckInspectorTypeExists(instance, &typeId))
            return;

        InspectorGUI::Property(propertyName);
        DrawTypeInspector(typeId, std::forward<entt::meta_any>(instance));
    }

    void Inspector::DrawTypeInspector(entt::id_type typeId, entt::meta_any&& instance) {
        using namespace entt::literals;

        auto& inspector = s_inspectorClasses[typeId];
        entt::meta_func fn;
        if (!CheckInspectorFunctionExists(CX_ON_INSPECTOR_GUI_HS, inspector, &fn))
            return;

        fn.invoke(inspector, (entt::meta_any* const)&instance, (const entt::meta_func::size_type)1);
    }

    void Inspector::DrawDefaultComponentInspector(entt::meta_any&& instance) {
        if (InspectorGUI::BeginPropertyTable("Default Inspector")) {
            auto type = instance.type();
            for (auto [id, data]: type.data()) {
                String label = Reflect::Core::GetFieldName(type, id);
                DrawPropertyInspector(
                    label,
                    Reflect::Core::CreateOpaqueReference(
                        data.type(),
                        Reflect::Core::GetFieldPointer(instance, id)
                    )
                );
            }
            InspectorGUI::EndPropertyTable();
        }
    }

    void Inspector::DrawComponentContextInspector(entt::meta_any&& component) {
        using namespace entt::literals;

        entt::id_type typeId;
        if (!CheckInspectorTypeExists(component, &typeId))
            return;

        auto& inspector = s_inspectorClasses[typeId];
        entt::meta_func fn;
        if (!CheckInspectorFunctionExists(CX_ON_INSPECTOR_CONTEXT_GUI_HS, inspector, &fn))
            return;

        if (ImGui::BeginPopupContextItem()) {
            fn.invoke(inspector, (entt::meta_any* const)&component, (const entt::meta_func::size_type)1);
            ImGui::EndPopup();
        }
    }

    bool Inspector::DrawComponentInspectorHeader(entt::meta_any&& instance) {
        String name = GetName(instance);
        bool header = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        DrawComponentContextInspector(std::forward<entt::meta_any>(instance));
        return header;
    }

    bool Inspector::CheckInspectorTypeExists(const entt::meta_any& instance, entt::id_type* typeId) {
        auto type = instance.type();
        auto id = type.id();

        // Check type is Ref<...>
        if (Reflect::Core::IsRefType(type)) {
            auto ptrTypeId = type.template_arg(0).id();
            if (s_inspectorClasses.count(ptrTypeId)) {
                if (typeId != nullptr)
                    *typeId = ptrTypeId;
                return true;
            }
        }

        if (!s_inspectorClasses.count(id))
            return false;

        if (typeId != nullptr)
            *typeId = id;

        return true;
    }

    bool Inspector::CheckInspectorFunctionExists(
        entt::id_type function,
        const entt::meta_any& inspector,
        entt::meta_func* fn
    ) {
        auto inspectorFn = inspector.type().func(function);
        if (!inspectorFn)
            return false;
        if (fn != nullptr)
            *fn = inspectorFn;
        return true;
    }

}