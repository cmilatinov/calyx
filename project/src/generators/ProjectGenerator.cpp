#include "generators/ProjectGenerator.h"
#include "generators/AssemblyGenerator.h"

#include "templates/RootCMake.h"
#include "templates/PrecompiledHeader.h"

#include <inja/inja.hpp>

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

        // Generate precompiled header
        std::ofstream pchStream(m_projectDirectory / "pch.h");
        if (!pchStream.is_open()) return false;
        pchStream << TEMPLATE_PRECOMPILED_HEADER << std::endl;

        return true;
    }

}