#include "inspector/Inspector.h"

#include "assets/Mesh.h"

#include <imgui.h>

namespace Calyx::Editor {

    CX_SINGLETON_INSTANCE(Inspector);

    Inspector::Inspector() {
        SelectionManager::Init();
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
                auto ptrType = Reflect::Core::GetRefPointerType(type);
                m_inspectorClasses[ptrType.id()] = instance;
                List<entt::meta_type> derivedClasses = Reflect::Core::GetDerivedClasses(ptrType);
                for (const auto& derived: derivedClasses) {
                    m_inspectorClasses[derived.id()] = instance;
                }
            } else {
                m_inspectorClasses[type.id()] = instance;
            }
        }
    }

    String Inspector::GetName(const entt::meta_any& instance) {
        String name;
        if (const auto* component = instance.try_cast<IComponent>(); component != nullptr) {
            name = component->GetName();
        } else {
            name = String(instance.type().info().name());
        }
        // TODO: Check for custom component name via instance.type().prop()
        return name;
    }

    void Inspector::_DrawComponentInspector(entt::meta_any& instance) {
        if (!DrawComponentInspectorHeader(instance))
            return;

        entt::id_type typeId;
        if (!CheckInspectorTypeExists(instance, &typeId))
            return DrawDefaultComponentInspector(instance);

        DrawTypeInspector(typeId, instance);
    }

    void Inspector::DrawPropertyInspector(const String& propertyName, entt::meta_any& instance) {
        entt::id_type typeId;
        if (!CheckInspectorTypeExists(instance, &typeId))
            return;

        InspectorGUI::Property(propertyName);
        DrawTypeInspector(typeId, instance);
    }

    void Inspector::DrawTypeInspector(entt::id_type typeId, entt::meta_any& instance) {
        using namespace entt::literals;

        auto& inspector = m_inspectorClasses[typeId];
        entt::meta_func fn;
        if (!CheckInspectorFunctionExists(CX_ON_INSPECTOR_GUI_HS, inspector, &fn))
            return;

        InvokeInspectorFunction(fn, inspector, instance);
    }

    void Inspector::DrawDefaultComponentInspector(entt::meta_any& instance) {
        if (InspectorGUI::BeginPropertyTable("Default Inspector")) {
            auto type = instance.type();
            for (auto [id, data]: type.data()) {
                String label = Reflect::Core::GetFieldName(type, id);
                auto ref = Reflect::Core::CreateOpaqueReference(
                    data.type(),
                    Reflect::Core::GetFieldPointer(instance, id)
                );
                DrawPropertyInspector(label, ref);
            }
            InspectorGUI::EndPropertyTable();
        }
    }

    void Inspector::DrawComponentContextInspector(entt::meta_any& component) {
        using namespace entt::literals;

        entt::id_type typeId;
        if (!CheckInspectorTypeExists(component, &typeId))
            return;

        auto& inspector = m_inspectorClasses[typeId];
        entt::meta_func fn;
        if (!CheckInspectorFunctionExists(CX_ON_INSPECTOR_CONTEXT_GUI_HS, inspector, &fn))
            return;

        if (ImGui::BeginPopupContextItem()) {
            InvokeInspectorFunction(fn, inspector, component);
            ImGui::EndPopup();
        }
    }

    bool Inspector::DrawComponentInspectorHeader(entt::meta_any& instance) {
        String name = GetName(instance);
        bool header = ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
        DrawComponentContextInspector(instance);
        return header;
    }

    bool Inspector::CheckInspectorTypeExists(const entt::meta_any& instance, entt::id_type* typeId) {
        auto type = instance.type();
        auto id = type.id();

        // Check type is Ref<...>
        if (Reflect::Core::IsRefType(type)) {
            auto ptrTypeId = Reflect::Core::GetRefPointerType(type).id();
            if (m_inspectorClasses.count(ptrTypeId)) {
                if (typeId != nullptr)
                    *typeId = ptrTypeId;
                return true;
            }
        }

        if (!m_inspectorClasses.count(id))
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

        fn.invoke(
            inspector,
            (entt::meta_any* const)args,
            (const entt::meta_func::size_type)(sizeof(args) / sizeof(*args))
        );

        if (isRefType) {
            const_cast<entt::meta_any&>(instance)
                .assign(type.construct((entt::meta_any* const)&args[1], (const entt::meta_func::size_type)1));
        }
    }

}