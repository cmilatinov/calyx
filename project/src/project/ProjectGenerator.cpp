#include "project/ProjectGenerator.h"
#include "assets/Assembly.h"
#include "templates/RootCMake.h"

namespace Calyx::Editor {

    ProjectGenerator::ProjectGenerator(const String& directory, const String& fileName, const String& name)
        : m_projectDirectory(directory),
          m_projectFile(m_projectDirectory / fileName),
          m_projectName(name) {}

    bool ProjectGenerator::Generate() {
        FileSystem::create_directories(m_projectDirectory);
        if (!FileSystem::is_empty(m_projectDirectory))
            return false;

        // Project Directories
        json directories = json::object(
            {
                { CX_PROJECT_DIRECTORY_ASSETS, CX_PROJECT_DIRECTORY_ASSETS_DEFAULT },
                { CX_PROJECT_DIRECTORY_BUILD, CX_PROJECT_DIRECTORY_BUILD_DEFAULT },
                { CX_PROJECT_DIRECTORY_CMAKE, CX_PROJECT_DIRECTORY_CMAKE_DEFAULT }
            }
        );

        for (const auto& directory: directories) {
            FileSystem::create_directories(m_projectDirectory / directory.get<String>());
        }

        // Project file
        std::ofstream outFile(m_projectFile);
        if (!outFile.is_open()) return false;
        json project = json::object(
            {
                { "name", m_projectName },
                { "directories", directories }
            }
        );
        outFile << std::setw(4) << project;

        // Templates
        inja::Environment env;
        inja::Template rootTemplate = env.parse(TEMPLATE_ROOT_CMAKE);

        // Generate CMake file
        json templateInput = json::object({{ "project", project }});
        std::ofstream rootStream(m_projectDirectory / "CMakeLists.txt");
        if (!rootStream.is_open()) return false;
        env.render_to(rootStream, rootTemplate, templateInput);

        // Generate default assembly
        auto assembly = Assembly::CreateDefaultAssembly(
            m_projectDirectory / CX_PROJECT_DIRECTORY_ASSETS_DEFAULT / "default.cxasm"
        );
        assembly.WriteFile();

        return true;
    }

}