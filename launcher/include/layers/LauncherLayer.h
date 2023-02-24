#pragma once

#include "core/Layer.h"
#include "meta/RecentProjects.h"
#include "ui/Widgets.h"

namespace Calyx::Launcher {

    class LauncherLayer : public ILayer {

    public:
        LauncherLayer();

        void OnAttach() override;
        void OnGUI() override;

        String GetName() const override { return "Launcher Layer"; }

    private:
        void BeginViewport();
        void EndViewport();

        bool TopActionBar();
        void OpenProjectButton();
        void CreateProjectButton();

        void RecentProjectList();
        void ProjectItem(const ProjectMeta& meta);

        void OpenProject(const String& file);

    private:
        bool m_updateProjectList = false;
        String m_searchFilter;
        List<ProjectMeta> m_recentProjectsFiltered;

        FileSelectionContext m_openProject;

        FileSelectionContext m_createProject;
        bool m_projectInfoLinked = true;
        String m_projectName, m_projectFile;

    };


}