#include "scene/serialization/SceneExporter.h"
#include "serialization/Serializer.h"
#include "ecs/GameObject.h"
#include "ecs/components/TransformComponent.h"

namespace Calyx {

    SceneExporter::SceneExporter(const Scene& scene)
        : m_scene(scene) {}

    void SceneExporter::Save(const String& file) {
        std::ofstream stream(file, std::ios_base::binary);
        if (!stream.is_open())
            return;

        SceneHeader header{
            .magic = CX_SCENE_MAGIC,
            .version = {
                .major = CX_SCENE_VERSION_MAJOR,
                .minor = CX_SCENE_VERSION_MINOR
            },
            .objectCount = m_scene.m_gameObjects.size()
        };
        Write(stream, header);

        for (const auto& [id, gameObject]: m_scene.m_gameObjects) {
            Serializer::Serialize(stream, gameObject->GetID());
            auto* parent = gameObject->GetParent();
            Serializer::Serialize(stream, parent != nullptr ? gameObject->GetParent()->GetID() : UUID());
            Serializer::Serialize(stream, gameObject->GetName());
        }

        for (const auto& [id, gameObject]: m_scene.m_gameObjects) {
            List<entt::id_type> components;
            for (const auto& [componentID, set]: m_scene.m_entityRegistry.storage()) {
                if (set.contains(id)) {
                    components.push_back(componentID);
                }
            }
            uint16 nComponents = components.size();
            Write(stream, nComponents);

            for (const auto& componentID: components) {
                auto type = entt::resolve(componentID);
                auto component = type.from_void(m_scene.m_entityRegistry.storage(componentID)->get(id));
                Write(stream, componentID);
                Serializer::Serialize(stream, component);
            }
        }
    }

}