#pragma once

#include <entt/entity/registry.hpp>

namespace Calyx {

    class Scene {

    public:



    private:
        entt::registry m_entityRegistry;

        friend class Entity;

    };

}