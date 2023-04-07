#include "EditorApp.h"
#include "layers/EditorLayer.h"
#include "project/ProjectManager.h"
#include "background/BackgroundTaskManager.h"

namespace Calyx::Editor {

    EditorApp::EditorApp(int argc, char** argv) {
        ProjectManager::Init();
        Inspector::Init();
        BackgroundTaskManager::Init();
        Window::GetMainWindow().SetTitle("Calyx Editor");

        cxxopts::Options options("Calyx Editor", "Calyx game engine editor application.");
        options.add_options()("file", "The project file to open.", cxxopts::value<String>()->default_value(""));
        options.parse_positional("file");
        auto args = options.parse(argc, argv);
        auto projectFile = args["file"].as<String>();
        if (projectFile.empty()) {
            CX_CORE_ERROR("No project file specified!");
            Application::Close();
            return;
        }

        CX_CORE_INFO("Opening project '{}' ...", projectFile);
        if (!ProjectManager::LoadProject(projectFile)) {
            CX_CORE_ERROR("Unable to parse project file!");
            Application::Close();
            return;
        }

        Window::GetMainWindow().SetTitle(fmt::format("Calyx Editor - {}", ProjectManager::GetProjectName()));
        PushUnderlay(new EditorLayer());
    }

}

namespace Calyx {

    Scope<Application> CreateApplication(int argc, char** argv) {
        return CreateScope<Editor::EditorApp>(argc, argv);
    }

}