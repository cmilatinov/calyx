#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <entt/meta/meta.hpp>

#include "utils.h"
#include "registration.h"
#include "annotations.h"

#define CX_REFLECT_FIELD_NAMES "field_names"_hs
#define CX_REFLECT_FIELD_OFFSETS "field_offsets"_hs

namespace Calyx::Reflect {

    class Core {
        using ClassMap = std::unordered_map<entt::id_type, std::vector<entt::meta_type>>;

    public:
        using FieldNameMap = std::unordered_map<entt::id_type, const char*>;
        using FieldOffsetMap = std::unordered_map<entt::id_type, int32_t>;

        static void RegisterDerivedClass(const entt::meta_type& base, const entt::meta_type& derived);

        template<typename T>
        static std::vector<entt::meta_type> GetDerivedClasses() {
            auto type = entt::resolve<T>();
            std::vector<entt::meta_type> derived;
            CollectDerivedClasses(type, derived);
            return derived;
        }

        static std::string GetFieldName(const entt::meta_type& type, entt::id_type fieldId);
        static int32_t GetFieldOffset(const entt::meta_type& type, entt::id_type fieldId);
        static void* GetFieldPointer(const entt::meta_any& instance, entt::id_type fieldId);

        template<typename T>
        static entt::meta_any CreateReference(T* ref) {
            return entt::resolve<T>().from_void(ref);
        }

        static entt::meta_any CreateOpaqueReference(const entt::meta_type& type, void* ref);

    private:
        static void CollectDerivedClasses(const entt::meta_type& type, std::vector<entt::meta_type>& derivedList);
        static ClassMap& GetDerivedClassMap();

    };

}