#include "scene/serialization/SceneImporter.h"
#include "scene/Scene.h"
#include "serialization/Serializer.h"
#include "ecs/GameObject.h"

namespace Calyx {

    Scope<Scene> SceneImporter::Load(const String& file) {
        std::ifstream stream(file);
        if (!stream.is_open())
            return Scope<Scene>();

        SceneHeader header{};
        Read(stream, header);
        if (header.magic != CX_SCENE_MAGIC)
            return Scope<Scene>();

        if (header.version.major != CX_SCENE_VERSION_MAJOR || header.version.minor != CX_SCENE_VERSION_MINOR)
            return Scope<Scene>();

        Scope<Scene> scene = CreateScope<Scene>();
        struct SceneObject {
            UUID parentID;
            GameObject* gameObject;
        };
        List<SceneObject> gameObjects;
        gameObjects.reserve(header.objectCount);
        for (int i = 0; i < header.objectCount; i++) {
            UUID id, parentID;
            String name;
            Serializer::Deserialize(stream, id);
            Serializer::Deserialize(stream, parentID);
            Serializer::Deserialize(stream, name);
            auto* gameObject = scene->CreateGameObject(name, nullptr, id);
            gameObjects.push_back(
                {
                    .parentID = parentID,
                    .gameObject = gameObject
                }
            );
        }

        for (int i = 0; i < header.objectCount; i++) {
            auto* gameObject = gameObjects[i].gameObject;
            uint16 nComponents;
            Read(stream, nComponents);
            for (int c = 0; c < nComponents; c++) {
                entt::id_type componentID;
                Read(stream, componentID);
                auto type = entt::resolve(componentID);
                auto instance = gameObject->AddComponent(type);
                CX_ASSERT(instance, "Failed to instantiate component!");
                if (instance) {
                    Serializer::Deserialize(stream, instance);
                }
            }
        }

        for (int i = 0; i < header.objectCount; i++) {
            auto& [parentID, gameObject] = gameObjects[i];
            if (!parentID.is_nil()) {
                gameObject->SetParent(scene->GetGameObject(parentID));
            }
        }

        return scene;
    }

}