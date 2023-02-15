#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <entt/entt.hpp>

#include "Registration.h"
#include "Annotations.h"

#define CX_REFLECT_FIELD_META "field_meta"_hs

namespace Calyx::Reflect {

    class Core {
        friend void ::CX_REFLECT_REGISTRATION_FN_NAME();

    private:
        struct FieldMeta {
            std::string name;
            std::string displayName;
            int32_t offset;
        };

        using ClassMap = std::unordered_map<entt::id_type, std::vector<entt::meta_type>>;
        using FieldMetaMap = std::unordered_map<entt::id_type, FieldMeta>;

    public:
        static void RegisterDerivedClass(const entt::meta_type& base, const entt::meta_type& derived);

        static std::vector<entt::meta_type> GetDerivedClasses(const entt::meta_type& type);

        template<typename T>
        inline static std::vector<entt::meta_type> GetDerivedClasses() {
            return GetDerivedClasses(entt::resolve<T>());
        }

        static std::string GetFieldName(const entt::meta_type& type, entt::id_type fieldId);
        static std::string GetFieldDisplayName(const entt::meta_type& type, entt::id_type fieldId);
        static int32_t GetFieldOffset(const entt::meta_type& type, entt::id_type fieldId);
        static void* GetFieldPointer(const entt::meta_any& instance, entt::id_type fieldId);

        template<typename T>
        static entt::meta_any CreateReference(T* ref) {
            return entt::resolve<T>().from_void(ref);
        }

        static entt::meta_any CreateOpaqueReference(const entt::meta_type& type, void* ref);

        static bool IsRefType(const entt::meta_type& type);
        static entt::meta_type GetRefPointerType(const entt::meta_type& type);

        static bool IsPointerType(const entt::meta_type& type);
        static entt::meta_type GetPointerType(const entt::meta_type& type);

    private:
        static ClassMap& GetDerivedClassMap();
        static const FieldMeta* GetFieldMeta(const entt::meta_type& type, entt::id_type fieldId);
        static void CollectDerivedClasses(const entt::meta_type& type, std::vector<entt::meta_type>& derivedList);

    };

}