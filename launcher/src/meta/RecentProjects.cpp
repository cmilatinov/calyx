#include "meta/RecentProjects.h"

namespace Calyx::Launcher {

    CX_SINGLETON_INSTANCE(RecentProjects);

    static const List<vec3> s_materialColors = {
        { CX_RGB_FROM_HEX(0xD32F2F) },
        { CX_RGB_FROM_HEX(0xD81B60) },
        { CX_RGB_FROM_HEX(0x9C27B0) },
        { CX_RGB_FROM_HEX(0x673AB7) },
        { CX_RGB_FROM_HEX(0x3F51B5) },
        { CX_RGB_FROM_HEX(0x1976D2) },
        { CX_RGB_FROM_HEX(0x0288D1) },
        { CX_RGB_FROM_HEX(0x0097A7) },
        { CX_RGB_FROM_HEX(0x009688) },
        { CX_RGB_FROM_HEX(0x43A047) },
        { CX_RGB_FROM_HEX(0xD84315) },
        { CX_RGB_FROM_HEX(0x607D8B) }
    };

    static std::random_device s_random{};

    RecentProjects::RecentProjects()
        : m_recentPath(PathUtils::GetUserHomeDirectory() / ".calyx" / "recent.json") {
        LoadProjects();
    }

    RecentProjects::~RecentProjects() {
        SaveProjects();
    }

    void RecentProjects::LoadProjects() {
        if (!FileSystem::exists(m_recentPath) || !FileSystem::is_regular_file(m_recentPath))
            return;

        std::ifstream file(m_recentPath);
        if (!file.is_open())
            return;

        json recentProjects = json::parse(file);
        if (!recentProjects.is_array())
            return;

        for (const auto& project: recentProjects) {
            if (!project.is_object())
                continue;

            m_projectList.push_back(
                {
                    .name = project["name"],
                    .directory = project["directory"],
                    .file = project["file"],
                    .color = {
                        project["color"][0],
                        project["color"][1],
                        project["color"][2]
                    }
                }
            );
        }
    }

    void RecentProjects::SaveProjects() {
        FileSystem::create_directories(m_recentPath.parent_path());
        std::ofstream file(m_recentPath);
        if (!file.is_open())
            return;

        json projects = json::array();
        for (const auto& project: m_projectList) {
            projects.push_back(
                {
                    { "name", project.name },
                    { "directory", project.directory },
                    { "file", project.file },
                    { "color", json::array({ project.color.x, project.color.y, project.color.z }) }
                }
            );
        }

        file << std::setw(4) << projects;
    }

    void RecentProjects::_AddProject(const String& name, const Path& file) {
        const int i = std::floor(
            s_materialColors.size() * (s_random() / std::abs((float)s_random.max() - s_random.min())));
        auto path = PathUtils::Normalize(file);
        m_projectList.push_back(
            {
                .name = name,
                .directory = path.parent_path(),
                .file = path,
                .color = s_materialColors[i]
            }
        );
    }

    void RecentProjects::_RemoveProject(const Path& file) {
        std::erase_if(m_projectList, [&file](const auto& project) { return project.file == file; });
    }

    List<ProjectMeta> RecentProjects::_SearchProjects(const String& query) {
        List<ProjectMeta> projects;
        for (const auto& project: m_projectList) {
            if (StringUtils::IsSearchMatch(query, project.name) || StringUtils::IsSearchMatch(query, project.file)) {
                projects.push_back(project);
            }
        }
        return projects;
    }

}