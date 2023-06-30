#pragma once

#include "project/ProjectDefinitions.h"
#include "project/ProjectInfo.h"
#include "assembly/AssemblyBuilder.h"

namespace Calyx::Editor {

    class ProjectManager {
    CX_SINGLETON(ProjectManager);

    public:
        ProjectManager();

        CX_SINGLETON_EXPOSE_METHOD(_LoadProject, bool LoadProject(const String& projectFile), projectFile);

        CX_SINGLETON_EXPOSE_METHOD(_GetProjectInfo, const ProjectInfo& GetProjectInfo());
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetDirectory, const Path& GetAssetDirectory());
        CX_SINGLETON_EXPOSE_METHOD(_GetBuildDirectory, const Path& GetBuildDirectory());
        CX_SINGLETON_EXPOSE_METHOD(_GetCMakeDirectory, const Path& GetCMakeDirectory());
        CX_SINGLETON_EXPOSE_METHOD(_GetProjectName, const String& GetProjectName());

    private:
        bool _LoadProject(const String& projectFile);

        const ProjectInfo& _GetProjectInfo() const { return m_projectInfo; }
        const Path& _GetAssetDirectory() const { return m_projectInfo.directories.assets; }
        const Path& _GetBuildDirectory() const { return m_projectInfo.directories.build; }
        const Path& _GetCMakeDirectory() const { return m_projectInfo.directories.cmake; }
        const String& _GetProjectName() const { return m_projectInfo.name; }

    private:
        ProjectInfo m_projectInfo;
        AssemblyBuilder m_assemblyBuilder;

    };

}