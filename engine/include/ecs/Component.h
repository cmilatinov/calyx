#pragma once

#include "ecs/GameObject.h"
#include "assets/Asset.h"

namespace Calyx {

    class Scene;

    class CALYX_API IComponent : public IAsset {
        CX_ASSET_REGISTRY_FRIEND();
        CX_BEGIN_REFLECT();

        friend class GameObject;
        friend class Scene;

    public:
        ~IComponent() override = default;

        virtual String GetDisplayName() const = 0;
        virtual String GetClassName() const = 0;
        virtual IdentifierType GetTypeId() const = 0;

    private:
        virtual entt::meta_any Emplace(entt::entity entity, entt::registry& registry) = 0;
        virtual void Erase(entt::entity entity, entt::registry& registry) = 0;

    };

    template<typename T>
    class CALYX_API Component : public IComponent {
        // TODO: Figure out how to enforce default constructors for subclasses
        // static_assert(std::is_default_constructible_v<T>, "T must be default constructible!");

        friend class GameObject;

    public:
        String GetClassName() const final {
            return String(NAMEOF_SHORT_TYPE(T));
        }

        String GetDisplayName() const override {
            return GetClassName();
        }

        IdentifierType GetTypeId() const override {
            auto type = entt::resolve<T>();
            // TODO: Fix this, its meant to check if T has custom reflection data
            // but its currently checking if the meta_type corresponding to T is
            // valid, which will always be the case
            if (!type) {
                CX_CORE_WARN(
                    "Script '{}' is not registered as reflected, its hook methods will not work correctly.\n"
                    "Make sure to run the reflection tool on its header file.", NAMEOF_SHORT_TYPE(T).data()
                );
                entt::meta<T>().type(entt::hashed_string(NAMEOF_SHORT_TYPE(T).data()));
                type = entt::resolve<T>();
            }
            return type.id();
        }

        AssetType GetAssetType() const override {
            return entt::resolve<T>().id();
        }

        template<typename C>
        C* GetComponent() const {
            return m_gameObject->GetComponent<C>();
        }

    protected:
        GameObject* m_gameObject = nullptr;

    private:
        entt::meta_any Emplace(entt::entity entity, entt::registry& registry) override {
            return entt::forward_as_any(registry.emplace<T>(entity));
        }

        void Erase(entt::entity entity, entt::registry& registry) override {
            registry.erase<T>(entity);
        }

    };

}