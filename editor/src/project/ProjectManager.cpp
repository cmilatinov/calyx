#include "project/ProjectManager.h"
#include "background/BackgroundTaskManager.h"

namespace Calyx::Editor {

    CX_SINGLETON_INSTANCE(ProjectManager);

    ProjectManager::ProjectManager()
        : m_assemblyBuilder(m_projectInfo) {}

    bool ProjectManager::_LoadProject(const String& projectFile) {
        std::ifstream stream(projectFile);
        if (!stream.is_open())
            return false;

        json project = json::parse(stream);
        m_projectInfo.projectFile = PathUtils::Normalize(projectFile);
        m_projectInfo.name = project[CX_PROJECT_NAME].get<String>();

        m_projectInfo.directories.project = m_projectInfo.projectFile.parent_path();
        m_projectInfo.directories.assets =
            m_projectInfo.directories.project /
            project[CX_PROJECT_DIRECTORIES][CX_PROJECT_DIRECTORY_ASSETS].get<String>();
        m_projectInfo.directories.build =
            m_projectInfo.directories.project /
            project[CX_PROJECT_DIRECTORIES][CX_PROJECT_DIRECTORY_BUILD].get<String>();
        m_projectInfo.directories.cmake =
            m_projectInfo.directories.project /
            project[CX_PROJECT_DIRECTORIES][CX_PROJECT_DIRECTORY_CMAKE].get<String>();

        AssetRegistry::AddSearchPath(m_projectInfo.directories.assets);

        BackgroundTaskManager::GetInstance().GetThreadPool().Enqueue(
            [this]() {
                m_assemblyBuilder.BuildAssemblies();
            }
        );

        return true;
    }

}