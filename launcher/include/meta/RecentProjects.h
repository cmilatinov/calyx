#pragma once

namespace Calyx::Launcher {

    struct ProjectMeta {
        String name;
        String directory;
        String file;
        vec3 color;
    };

    class RecentProjects {
        CX_SINGLETON(RecentProjects);

    public:
        RecentProjects();
        ~RecentProjects();

    public:
        CX_SINGLETON_EXPOSE_METHOD(_SearchProjects, List<ProjectMeta> SearchProjects(const String& query), query);
        CX_SINGLETON_EXPOSE_METHOD(_AddProject, void AddProject(const String& name, const Path& file), name, file);

    private:
        void LoadProjects();
        void SaveProjects();

        void _AddProject(const String& name, const Path& file);

        List<ProjectMeta> _SearchProjects(const String& query);

    private:
        Path m_recentPath;
        List<ProjectMeta> m_projectList;

    };

}