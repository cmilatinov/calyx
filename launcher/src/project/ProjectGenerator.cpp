#include "project/ProjectGenerator.h"

namespace Calyx::Launcher {

    ProjectGenerator::ProjectGenerator(const String& directory, const String& fileName, const String& name)
        : m_projectDirectory(directory),
          m_projectFile(m_projectDirectory / fileName),
          m_projectName(name) {}

    bool ProjectGenerator::Generate() {
        FileSystem::create_directories(m_projectDirectory);
        if (!FileSystem::is_empty(m_projectDirectory))
            return false;

        std::ofstream outFile(m_projectFile);
        if (!outFile.is_open())
            return false;

        json directories = json::object({
            { "assets", "GameAssets" },
            { "build", "build" }
        });

        for (const auto& directory: directories) {
            FileSystem::create_directories(m_projectDirectory / directory.get<String>());
        }

        json project = json::object({
            { "name", m_projectName },
            { "directories", directories }
        });

        outFile << project.dump(4);
        return true;
    }

}