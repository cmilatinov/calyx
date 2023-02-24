#include "assembly/AssemblyBuilder.h"
#include "templates/AssembliesCMake.h"

namespace Calyx::Editor {

    AssemblyBuilder::AssemblyBuilder(const ProjectInfo& projectInfo)
        : m_project(projectInfo) {}

    void AssemblyBuilder::BuildAssemblies() {
        bool regenerateCMake = false;
        for (const auto& assembly: m_project.assemblies) {
            auto sourceFileList = FileUtils::GlobRecursive(m_project.directories.assets, assembly->GetSources());
            Set<String> sourceFileSet;
            for (const auto& sourceFile: sourceFileList) {
                sourceFileSet.insert(FileSystem::relative(sourceFile, m_project.directories.project).string());
            }
            if (assembly->GetSourceFiles() != sourceFileSet) {
                regenerateCMake = true;
                break;
            }
        }

        Path assembliesCMake = m_project.directories.cmake / "assemblies.cmake";
        regenerateCMake |= !FileSystem::is_regular_file(assembliesCMake);

        if (regenerateCMake) {
            GenerateAssembliesCMake();
        }

        InvokeBuildSystem();
    }

    void AssemblyBuilder::GenerateAssembliesCMake() {
        Path assembliesCMake = m_project.directories.cmake / "assemblies.cmake";
        std::ofstream assemblies(assembliesCMake);
        if (!assemblies.is_open()) return;
        json assemblyList = json::array();
        for (const auto& assembly: m_project.assemblies) {
            assemblyList.push_back(assembly->ToJSON());
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

    void AssemblyBuilder::InvokeBuildSystem() {
        namespace bp = boost::process;
        bp::ipstream is;
        bp::child process(
            bp::search_path("cmake"),
            "-S", m_project.directories.project.string(),
            "-B", m_project.directories.build.string(),
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