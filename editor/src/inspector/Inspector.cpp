#include "inspector/Inspector.h"

#include "reflect/ClassRegistry.h"
#include "ecs/components/TransformComponent.h"

#include <imgui.h>

namespace Calyx::Editor {

    CX_SINGLETON_INSTANCE(Inspector);

    Inspector::Inspector() {
        SelectionManager::Init();
        InitializeTypes();
    }

    void Inspector::RegisterRefTypeProcessor(const entt::meta_type& type, const entt::meta_any& instance) {
        m_typeProcessors[type.id()] = instance;
        List<entt::meta_type> derivedClasses = Reflect::Core::GetDerivedClasses(type);

        // TODO: Find a way around this
        for (const auto& [_, info]: ClassRegistry::GetComponentClasses()) {
            auto it = std::remove(derivedClasses.begin(), derivedClasses.end(), info.type);
            derivedClasses.erase(it);
        }

        for (const auto& derived: derivedClasses) {
            m_typeMap[derived.id()] = type.id();
        }
    }

    String Inspector::GetName(const entt::meta_any& instance) {
        String name;
        if (const auto* component = instance.try_cast<IComponent>(); component != nullptr) {
            name = component->GetDisplayName();
        } else {
            name = String(instance.type().info().name());
        }
        // TODO: Check for custom component name via instance.type().prop()
        return name;
    }

    void Inspector::_DrawComponentInspector(GameObject* gameObject, entt::meta_any& componentInstance) {
        if (!DrawComponentInspectorHeader(gameObject, componentInstance))
            return;

        entt::meta_any* inspector;
        if (!CheckProcessorExists(componentInstance, &inspector))
            return DrawDefaultComponentInspector(componentInstance);

        DrawTypeInspector(*inspector, componentInstance);
    }

    void Inspector::DrawPropertyInspector(const String& propertyName, entt::meta_any& instance) {
        entt::meta_any* inspector;
        if (!CheckProcessorExists(instance, &inspector))
            return;

        InspectorGUI::Property(propertyName);
        DrawTypeInspector(*inspector, instance);
    }

    void Inspector::DrawTypeInspector(entt::meta_any& inspector, entt::meta_any& instance) {
        using namespace entt::literals;

        entt::meta_func fn;
        if (!CheckProcessorFunctionExists(CX_ON_INSPECTOR_GUI_HS, inspector, &fn))
            return;

        InvokeInspectorFunction(fn, inspector, instance);
    }

    void Inspector::DrawDefaultComponentInspector(entt::meta_any& instance) {
        if (InspectorGUI::BeginPropertyTable("Default Inspector")) {
            auto type = instance.type();
            for (auto [id, data]: type.data()) {
                String label = Reflect::Core::GetFieldDisplayName(type, id);
                auto ref = Reflect::Core::CreateOpaqueReference(
                    data.type(),
                    Reflect::Core::GetFieldPointer(instance, id)
                );
                DrawPropertyInspector(label, ref);
            }
            InspectorGUI::EndPropertyTable();
        }
    }

    bool Inspector::DrawComponentContextInspector(GameObject* gameObject, entt::meta_any& component) {
        using namespace entt::literals;

        bool removed = false;
        if (ImGui::BeginPopupContextItem()) {
            if (component.type().id() != entt::resolve<TransformComponent>().id() && ImGui::MenuItem("Remove")) {
                removed = true;
            }

            entt::meta_any* inspector;
            entt::meta_func fn;
            if (CheckProcessorExists(component, &inspector) &&
                CheckProcessorFunctionExists(CX_ON_INSPECTOR_CONTEXT_GUI_HS, *inspector, &fn)) {
                InvokeInspectorFunction(fn, *inspector, component);
            }

            ImGui::EndPopup();
        }

        if (removed) {
            gameObject->RemoveComponent(component.type());
        }

        return !removed;
    }

    bool Inspector::DrawComponentInspectorHeader(GameObject* gameObject, entt::meta_any& instance) {
        String name = GetName(instance);
        bool header = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        bool context = DrawComponentContextInspector(gameObject, instance);
        return header && context;
    }

    void Inspector::InvokeInspectorFunction(
        const entt::meta_func& fn,
        entt::meta_any& inspector,
        entt::meta_any& instance
    ) {
        auto type = instance.type();
        bool isRefType = Reflect::Core::IsRefType(type);
        entt::meta_any args[2] = {
            isRefType ? Reflect::Core::GetRefPointerType(type).id() : type.id(),
            instance.as_ref()
        };

        if (isRefType) {
            args[1] = type.construct((entt::meta_any* const)&args[1], (const entt::meta_func::size_type)1);
        }

        const auto arity = fn.arity();
        if (arity == 1) {
            fn.invoke(
                inspector,
                (entt::meta_any* const)&args[1],
                (const entt::meta_func::size_type)1
            );
        } else if (arity == 2) {
            fn.invoke(
                inspector,
                (entt::meta_any* const)args,
                (const entt::meta_func::size_type)2
            );
        }

        if (isRefType) {
            const_cast<entt::meta_any&>(instance)
                .assign(type.construct((entt::meta_any* const)&args[1], (const entt::meta_func::size_type)1));
        }
    }

}