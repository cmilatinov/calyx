#include "scene/serialization/SceneExporter.h"
#include "reflect/ClassRegistry.h"
#include "serialization/Serializer.h"
#include "ecs/GameObject.h"

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
        stream.write(CharPtrCast(&header), sizeof(header));

        for (const auto& [_, gameObject]: m_scene.m_gameObjects) {
            auto& id = gameObject->GetID();
            auto& name = gameObject->GetName();
            stream.write(CharPtrCast(&id), sizeof(id));
            stream.write(name.c_str(), name.size() + 1);
            for (const auto& component: ClassRegistry::GetComponentClasses()) {
                if (gameObject->HasComponent(component)) {
                    auto componentId = component.id();
                    CX_CORE_TRACE("{} - {:x}", component.info().name(), componentId);
                    stream.write(CharPtrCast(&componentId), sizeof(componentId));
                    auto comp = component.from_void(m_scene.m_entityRegistry.storage(component.id())->get(gameObject->m_entityID));
                    Serializer::Serialize(stream, comp);
                }
            }
        }
    }

}