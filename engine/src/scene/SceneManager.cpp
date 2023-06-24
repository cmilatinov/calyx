#include "scene/SceneManager.h"
#include "scene/serialization/SceneImporter.h"
#include "assets/Mesh.h"
#include "ecs/components/MeshComponent.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(SceneManager);

    void SceneManager::_LoadEmptyScene() {
        StopSimulation();
        m_currentScene = CreateScope<Scene>();
    }

    void SceneManager::_LoadDefaultScene() {
        StopSimulation();
        m_currentScene = CreateScope<Scene>();

        auto cubeMesh = AssetRegistry::LoadAsset<Mesh>("meshes/cube.obj");

        auto* cube1 = m_currentScene->CreateGameObject("Cube 1");
        cube1->AddComponent<MeshComponent>()->SetMesh(cubeMesh);
        cube1->GetTransform().Translate(vec3(1, 0, 0));

        auto* cube2 = m_currentScene->CreateGameObject("Cube 2");
        cube2->AddComponent<MeshComponent>()->SetMesh(cubeMesh);
        cube2->GetTransform().Translate(vec3(0, 2, 0));
        cube2->SetParent(cube1);
    }

    void SceneManager::_LoadScene(const String& sceneFile) {
        StopSimulation();
        SceneImporter importer;
        m_currentScene = importer.Load(sceneFile);
        if (!m_currentScene) {
            m_currentScene = CreateScope<Scene>();
        }
    }

    void SceneManager::_UnloadCurrentScene() {
        m_simulationScene.reset();
        m_currentScene.reset();
    }

    void SceneManager::StartSimulation() {
        if (!m_simulationLoaded) {
            m_simulationScene = CreateScope<Scene>(*m_currentScene);
        }
        m_simulationLoaded = true;
        m_simulationRunning = true;
    }

    void SceneManager::PauseSimulation() {
        m_simulationRunning = false;
    }

    void SceneManager::StopSimulation() {
        m_simulationScene.reset();
        m_simulationRunning = false;
        m_simulationLoaded = false;
    }

    void SceneManager::Update() {
        if (!m_simulationRunning)
            return;
        m_simulationScene->Update();
    }

}