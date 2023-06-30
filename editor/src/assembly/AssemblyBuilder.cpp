#include "assembly/AssemblyBuilder.h"
#include "templates/AssembliesCMake.h"
#include "reflect/ClassRegistry.h"

#include <inja/inja.hpp>
#include <boost/process.hpp>

namespace Calyx::Editor {

    AssemblyBuilder::AssemblyBuilder(const ProjectInfo& projectInfo)
        : m_project(projectInfo) {}

    void AssemblyBuilder::BuildAssemblies() {
        LoadAssemblyDefinitions();

        CX_CORE_INFO("Generating assemblies CMake file ...");
        GenerateAssembliesCMake();

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
        for (const auto& assembly: m_assemblies) {
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

    void AssemblyBuilder::LoadAssemblyDefinitions() {
        m_assemblies.clear();
        m_assemblies.insert(CreateRef<AssemblyDefinition>("default", m_project.directories.assets));
        m_sourceFileAssemblies.clear();
        m_assemblyFiles.clear();
        auto assemblyFiles = FileUtils::GlobRecursive(m_project.directories.assets, { "**.cxasm" });
        for (const auto& file: assemblyFiles) {
            String assemblyAsset = FileSystem::relative(file, m_project.directories.assets);
            auto assembly = AssetRegistry::LoadAsset<AssemblyDefinition>(assemblyAsset);
            if (assembly) {
                m_assemblies.insert(std::forward<Ref<AssemblyDefinition>>(assembly));
            }
        }

        auto sources = FileUtils::GlobRecursive(m_project.directories.assets, { "**.cpp", "**.h", "**.hpp" });
        for (const auto& file: sources) {
            Ref<AssemblyDefinition> closest;
            for (const auto& assembly: m_assemblies) {
                auto& dir = assembly->GetDirectory();
                if (FileUtils::IsInDirectory(dir, file) &&
                    (!closest || dir.string().length() > closest->GetDirectory().string().length())) {
                    closest = assembly;
                }
            }
            auto src = FileSystem::relative(file, m_project.directories.project).string();
            m_sourceFileAssemblies[src] = closest.get();
            m_assemblyFiles[closest.get()].insert(file);
        }
    }

    void AssemblyBuilder::GenerateAssembliesCMake() {
        Path assembliesCMake = m_project.directories.cmake / "assemblies.cmake";
        std::ofstream assemblies(assembliesCMake);
        if (!assemblies.is_open()) return;
        json assemblyList = json::array();
        for (const auto& assembly: m_assemblies) {
            json sourceFiles = json::array();
            json headerFiles = json::array();
            for (const auto& file: m_assemblyFiles[assembly.get()]) {
                if (file.ends_with(".cpp")) {
                    sourceFiles.push_back(file);
                } else if (file.ends_with(".h") || file.ends_with(".hpp")) {
                    headerFiles.push_back(file);
                }
            }
            assemblyList.push_back(
                json::object(
                    {
                        { CX_ASSEMBLY_NAME, assembly->GetName() },
                        { CX_ASSEMBLY_SOURCE_FILES, sourceFiles },
                        { CX_ASSEMBLY_HEADER_FILES, headerFiles }
                    }
                )
            );
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