#include "windows/GamePanel.h"
//#include "scene/SceneManager.h"

#include <imgui.h>

namespace Calyx::Editor {

    void GamePanel::Draw() {
//        m_scene = SceneManager::GetSimulationOrCurrentScene();

        ImGui::Begin("Game");

        ImGui::End();
    }

}