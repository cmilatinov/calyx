#include "Reflect.h"

namespace Calyx::Reflect {

    void Core::RegisterDerivedClass(const entt::meta_type& base, const entt::meta_type& derived) {
        auto& derivedClasses = GetDerivedClassMap();
        if (derived) {
            derivedClasses[base.id()].push_back(derived);
        }
    }

    std::vector<entt::meta_type> Core::GetDerivedClasses(const entt::meta_type& type) {
        std::vector<entt::meta_type> derived;
        CollectDerivedClasses(type, derived);
        return derived;
    }

    void Core::CollectDerivedClasses(const entt::meta_type& type, std::vector<entt::meta_type>& derivedList) {
        auto& derivedClasses = GetDerivedClassMap();
        for (auto& derived: derivedClasses[type.id()]) {
            derivedList.push_back(derived);
            CollectDerivedClasses(derived, derivedList);
        }
    }

    std::string Core::GetFieldName(const entt::meta_type& type, entt::id_type fieldId) {
        const auto* meta = GetFieldMeta(type, fieldId);
        if (meta == nullptr)
            return "";
        return meta->name;
    }

    std::string Core::GetFieldDisplayName(const entt::meta_type& type, entt::id_type fieldId) {
        const auto* meta = GetFieldMeta(type, fieldId);
        if (meta == nullptr)
            return "";
        return meta->displayName;
    }

    int32_t Core::GetFieldOffset(const entt::meta_type& type, entt::id_type fieldId) {
        const auto* meta = GetFieldMeta(type, fieldId);
        if (meta == nullptr)
            return -1;
        return meta->offset;
    }

    void* Core::GetFieldPointer(const entt::meta_any& instance, entt::id_type fieldId) {
        void* instancePtr = const_cast<void*>(instance.data());

        int32_t fieldOffset = GetFieldOffset(instance.type(), fieldId);
        if (fieldOffset < 0)
            return nullptr;

        return static_cast<uint8_t*>(instancePtr) + fieldOffset;
    }

    entt::meta_any Core::CreateOpaqueReference(const entt::meta_type& type, void* ref) {
        return type.from_void(ref);
    }

    bool Core::IsRefType(const entt::meta_type& type) {
        static const auto sharedPtrTypeId = entt::resolve<entt::meta_class_template_tag<std::shared_ptr>>().id();
        return type.is_pointer_like() && type.template_type().id() == sharedPtrTypeId;
    }

    entt::meta_type Core::GetRefPointerType(const entt::meta_type& type) {
        if (!IsRefType(type))
            return entt::meta_type();
        return type.template_arg(0);
    }

    bool Core::IsPointerType(const entt::meta_type& type) {
        return type.is_pointer();
    }

    entt::meta_type Core::GetPointerType(const entt::meta_type& type) {
        return type.remove_pointer();
    }

    const Core::FieldMeta* Core::GetFieldMeta(const entt::meta_type& type, entt::id_type fieldId) {
        using namespace entt::literals;

        auto fieldMetaAny = type.prop(CX_REFLECT_FIELD_META).value();
        if (!fieldMetaAny)
            return nullptr;

        const auto* fieldMeta = fieldMetaAny.try_cast<const FieldMetaMap>();
        if (fieldMeta == nullptr || !fieldMeta->count(fieldId))
            return nullptr;

        return &fieldMeta->at(fieldId);
    }

    Core::ClassMap& Core::GetDerivedClassMap() {
        static ClassMap map{};
        return map;
    }

}