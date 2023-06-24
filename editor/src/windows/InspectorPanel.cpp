#include "windows/InspectorPanel.h"
#include "inspector/SelectionManager.h"
#include "inspector/Inspector.h"
#include "reflect/ClassRegistry.h"
#include "scene/SceneManager.h"
#include "ui/Widgets.h"

#include <imgui.h>

namespace Calyx::Editor {

    void InspectorPanel::Draw() {
        ImGui::Begin("Inspector");
        // TODO: Handle multiple object selection
        auto& selection = SelectionManager::GetCurrentSelection();
        if (selection.IsGameObjectSelection() && !selection.IsEmpty()) {
            auto* selected = selection.First().try_cast<GameObject>();

            // Name & ID
            if (InspectorGUI::BeginPropertyTable("GameObject")) {
                InspectorGUI::Property("ID");
                String id = uuids::to_string(selected->GetID());
                InspectorGUI::TextControl("##id", id, true);

                InspectorGUI::Property("Name");
                String name = selected->GetName();
                if (InspectorGUI::TextControl("##name", name)) {
                    selected->SetName(name);
                }

                InspectorGUI::EndPropertyTable();
            }

            auto* scene = SceneManager::GetSimulationOrCurrentScene();

            // Components
            Set<AssetType> componentSet;
            for (const auto& [id, info]: ClassRegistry::GetComponentClasses()) {
                auto storage = scene->m_entityRegistry.storage(id);
                if (storage != nullptr && storage->contains(selected->m_entityID)) {
                    Inspector::DrawComponentInspector(selected, info.type, storage->get(selected->m_entityID));
                    componentSet.insert(id);
                }
            }

            // Add components
            InspectorGUI::ComponentPicker(selected, componentSet);
        }
        ImGui::End();
    }

}