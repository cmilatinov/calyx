#include "reflect.h"

namespace Calyx::Reflect {

    void Core::RegisterDerivedClass(const entt::meta_type& base, const entt::meta_type& derived) {
        auto& derivedClasses = GetDerivedClassMap();
        if (derived) {
            derivedClasses[base.id()].push_back(derived);
        }
    }

    void Core::CollectDerivedClasses(const entt::meta_type& type, std::vector<entt::meta_type>& derivedList) {
        auto& derivedClasses = GetDerivedClassMap();
        derivedList.reserve(derivedClasses.size());
        for (auto& derived: derivedClasses[type.id()]) {
            derivedList.push_back(derived);
            CollectDerivedClasses(derived, derivedList);
        }
    }

    std::string Core::GetFieldName(const entt::meta_type& type, entt::id_type fieldId) {
        using namespace entt::literals;

        auto fieldNamesAny = type.prop(CX_REFLECT_FIELD_NAMES).value();
        if (!fieldNamesAny)
            return "";

        auto fieldNames = fieldNamesAny.cast<FieldNameMap>();
        if (!fieldNames.count(fieldId))
            return "";

        return fieldNames.at(fieldId);
    }

    int32_t Core::GetFieldOffset(const entt::meta_type& type, entt::id_type fieldId) {
        using namespace entt::literals;

        auto fieldOffsetsAny = type.prop(CX_REFLECT_FIELD_OFFSETS).value();
        if (!fieldOffsetsAny)
            return -1;

        auto fieldOffsets = fieldOffsetsAny.cast<FieldOffsetMap>();
        if (!fieldOffsets.count(fieldId))
            return -1;

        return fieldOffsets.at(fieldId);
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

    Core::ClassMap& Core::GetDerivedClassMap() {
        static ClassMap map;
        return map;
    }

}