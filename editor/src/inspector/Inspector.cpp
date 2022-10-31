#include "inspector/Inspector.h"
#include "math/Transform.h"

namespace Calyx::Editor {

    Inspector::InspectorMap Inspector::s_inspectorClasses{};

    void Inspector::Init() {
        s_inspectorClasses.clear();
        List<entt::meta_type> inspectorClasses = Reflect::Core::GetDerivedClasses<ITypeInspector>();
        for (auto& inspector: inspectorClasses) {
            auto instance = inspector.construct();
            if (!instance)
                continue;

            auto* typeInspector = instance.try_cast<ITypeInspector>();
            if (typeInspector == nullptr) {
                instance.reset();
                continue;
            }

            s_inspectorClasses[typeInspector->GetMetaType().id()] = instance;
        }
    }

    String Inspector::GetName(const entt::meta_any& instance) {
        String name = String(instance.type().info().name());
        const IComponent* component;
        if ((component = instance.try_cast<IComponent>()) != nullptr) {
            name = component->GetName();
        }
        // TODO: Check for custom component name via instance.type().prop()
        return name;
    }

    void Inspector::DrawComponentInspector(entt::meta_any&& instance) {
        if (!DrawComponentInspectorHeader(std::forward<entt::meta_any>(instance)))
            return;

        if (!CheckInspectorTypeExists(instance))
            return DrawDefaultComponentInspector(std::forward<entt::meta_any>(instance));

        DrawPropertyInspector(std::forward<entt::meta_any>(instance));
    }

    void Inspector::DrawPropertyInspector(entt::meta_any&& instance, const char* label) {
        using namespace entt::literals;

        entt::id_type typeId;
        if (!CheckInspectorTypeExists(instance, &typeId))
            return;

        auto& inspector = s_inspectorClasses[typeId];
        entt::meta_func fn;
        if (!CheckInspectorFunctionExists(CX_ON_INSPECTOR_GUI_HS, inspector, &fn))
            return;

        fn.invoke(inspector, (entt::meta_any* const) &instance, (const entt::meta_func::size_type) 1);
    }

    void Inspector::DrawDefaultComponentInspector(entt::meta_any&& instance) {
        auto type = instance.type();
        for (auto [id, data]: type.data()) {
            String label = Reflect::Core::GetFieldName(type, id);
            auto member = Reflect::Core::CreateOpaqueReference(
                data.type(),
                Reflect::Core::GetFieldPointer(instance, id)
            );
            DrawPropertyInspector(std::forward<entt::meta_any>(member), label.c_str());
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
            fn.invoke(inspector, (entt::meta_any* const) &component, (const entt::meta_func::size_type) 1);
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
        auto id = instance.type().id();
        if (!s_inspectorClasses.count(instance.type().id()))
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