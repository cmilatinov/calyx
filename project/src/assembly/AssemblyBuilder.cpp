#include "assembly/AssemblyBuilder.h"
#include "templates/AssembliesCMake.h"
#include "reflect/ClassRegistry.h"

#include <inja/inja.hpp>
#include <boost/process.hpp>

namespace Calyx::Editor {

    AssemblyBuilder::AssemblyBuilder(const ProjectInfo& projectInfo)
        : m_project(projectInfo) {}

    void AssemblyBuilder::BuildAssemblies() {
        bool regenerateCMake = UpdateAssemblies();
        Path assembliesCMake = m_project.directories.cmake / "assemblies.cmake";
        regenerateCMake |= !FileSystem::exists(assembliesCMake) || !FileSystem::is_regular_file(assembliesCMake);

        if (regenerateCMake) {
            CX_CORE_INFO("Generating assemblies CMake file ...");
            GenerateAssembliesCMake();
        }

        CX_CORE_INFO("Configuring CMake project ...");
        ConfigureBuildSystem();

        CX_CORE_INFO("Building assemblies ...");
        InvokeBuildSystem();

        CX_CORE_INFO("Reloading project assemblies ...");
        LoadAssemblies();

        CX_CORE_INFO("Refreshing class lists ...");
        ClassRegistry::GetInstance().RefreshClassLists();

        CX_CORE_INFO("Updating component asset info ...");
        AssetRegistry::GetInstance().RegisterComponents();
    }

    void AssemblyBuilder::LoadAssemblies() {
        using namespace entt::literals;
        CX_CORE_INFO(
            "Unloading {} currently loaded assembl{} ...",
            m_loadedAssemblies.size(),
            m_loadedAssemblies.size() == 1 ? "y" : "ies"
        );
        m_loadedAssemblies.clear();

        CX_CORE_INFO("Loading built assemblies ...");
        for (const auto& assembly: m_project.assemblies) {
            auto fileName = assembly->GetBinaryName();
            Path binary = m_project.directories.build / fileName;
            if (FileSystem::exists(binary) && FileSystem::is_regular_file(binary)) {
                auto& lib = m_loadedAssemblies[fileName] = boost::dll::shared_library(binary);
                if (lib.is_loaded()) {
                    CX_CORE_INFO("Loaded assembly '{}'", assembly->GetName());
                } else {
                    CX_CORE_ERROR("Failed to load assembly '{}'", assembly->GetName());
                }
            }
        }
    }

    bool AssemblyBuilder::UpdateAssemblies() {
        bool regenerate = false;
        for (const auto& assembly: m_project.assemblies) {
            auto sourceFileList = FileUtils::GlobRecursive(m_project.directories.assets, assembly->GetSources());
            Set<String> sourceFileSet;
            std::transform(
                sourceFileList.begin(), sourceFileList.end(),
                std::inserter(sourceFileSet, sourceFileSet.begin()),
                [this](const auto& sourceFile) {
                    return FileSystem::relative(sourceFile, m_project.directories.project).string();
                }
            );
            if (assembly->GetSourceFiles() != sourceFileSet) {
                assembly->SetSourceFiles(m_project.directories.project, sourceFileSet);
                assembly->WriteFile();
                regenerate = true;
            }
        }
        return regenerate;
    }

    void AssemblyBuilder::GenerateAssembliesCMake() {
        Path assembliesCMake = m_project.directories.cmake / "assemblies.cmake";
        std::ofstream assemblies(assembliesCMake);
        if (!assemblies.is_open()) return;
        json assemblyList = json::array();
        for (const auto& assembly: m_project.assemblies) {
            if (!assembly->GetSourceFiles().empty()) {
                assemblyList.push_back(assembly->ToJSON());
            }
        }
        inja::Environment env;
        inja::Template templ = env.parse(TEMPLATE_ASSEMBLIES_CMAKE);
        env.render_to(
            assemblies, templ, json::object(
                {
                    {
                        "asset_directory",
                        FileSystem::relative(m_project.directories.assets, m_project.directories.project).string()
                    },
                    { "assemblies", assemblyList }
                }
            )
        );
    }

    void AssemblyBuilder::ConfigureBuildSystem() {
        namespace bp = boost::process;
        bp::ipstream is;
        bp::child process(
            bp::search_path("cmake"),
            "-S", m_project.directories.project.string(),
            "-B", m_project.directories.build.string(),
            "-DCMAKE_TOOLCHAIN_FILE=" CX_CMAKE_TOOLCHAIN_FILE,
            bp::std_out > is
        );

        String line;
        while (process.running() && std::getline(is, line) && !line.empty()) {
            CX_CORE_TRACE(line);
        }

        process.wait();
        process.exit_code();
    }

    void AssemblyBuilder::InvokeBuildSystem() {
        namespace bp = boost::process;
        bp::ipstream is;
        bp::child process(
            bp::search_path("cmake"),
            "--build", m_project.directories.build.string(),
            bp::std_out > is
        );

        String line;
        while (process.running() && std::getline(is, line) && !line.empty()) {
            CX_CORE_TRACE(line);
        }

        process.wait();
        process.exit_code();
    }

}