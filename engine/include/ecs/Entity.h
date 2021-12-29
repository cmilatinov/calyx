#pragma once

#include <entt/entity/registry.hpp>

namespace Calyx {

    class Entity {

        template<typename T>
        T& AddComponent(T& component);

        template<typename T>
        void RemoveComponent();

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent();

        operator bool()

        bool operator==(const Entity& other);
        bool operator!=(const Entity& other);

    private:
        entt::entity m_entityID;

    };

}