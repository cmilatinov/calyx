#pragma once

#include "scene/Scene.h"

namespace Calyx {

    class SceneManager {
        CX_SINGLETON(SceneManager);

    public:
        SceneManager() = default;

        CX_SINGLETON_EXPOSE_METHOD(_LoadEmptyScene, void LoadEmptyScene());
        CX_SINGLETON_EXPOSE_METHOD(_LoadDefaultScene, void LoadDefaultScene());
        CX_SINGLETON_EXPOSE_METHOD(_LoadScene, void LoadScene(const String& sceneFile), sceneFile);
        CX_SINGLETON_EXPOSE_METHOD(_UnloadCurrentScene, void UnloadCurrentScene());

        CX_SINGLETON_EXPOSE_METHOD(_GetCurrentScene, Scene* GetCurrentScene());
        CX_SINGLETON_EXPOSE_METHOD(_GetSimulationScene, Scene* GetSimulationScene());
        CX_SINGLETON_EXPOSE_METHOD(_GetSimulationOrCurrentScene, Scene* GetSimulationOrCurrentScene());

    public:
        void StartSimulation();
        void PauseSimulation();
        void StopSimulation();

        void Update();

        bool CanStartSimulation() const { return !m_simulationRunning; }
        bool CanPauseSimulation() const { return m_simulationRunning; }
        bool CanStopSimulation() const { return m_simulationLoaded || m_simulationRunning; }

    private:
        void _LoadEmptyScene();
        void _LoadDefaultScene();
        void _LoadScene(const String& sceneFile);
        void _UnloadCurrentScene();

        Scene* _GetCurrentScene() const { return m_currentScene.get(); }
        Scene* _GetSimulationScene() const { return m_simulationScene.get(); }
        Scene* _GetSimulationOrCurrentScene() const {
            return static_cast<bool>(m_simulationScene) ? m_simulationScene.get() : m_currentScene.get();
        }

    private:
        Scope<Scene> m_currentScene = CreateScope<Scene>();

        // Simulation
        bool m_simulationRunning = false;
        bool m_simulationLoaded = false;
        Scope<Scene> m_simulationScene;

    };

}