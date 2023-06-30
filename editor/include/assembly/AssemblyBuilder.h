#pragma once

#include "project/ProjectInfo.h"
#include "assembly/AssemblyDefinition.h"

#include <boost/dll/shared_library.hpp>

namespace Calyx::Editor {

    class AssemblyBuilder {

    public:
        explicit AssemblyBuilder(const ProjectInfo& projectInfo);

        void BuildAssemblies();
        void LoadAssemblies();

    private:
        void LoadAssemblyDefinitions();
        void GenerateAssembliesCMake();
        void ConfigureBuildSystem();
        void InvokeBuildSystem();

        // TODO: Implement
        void OnAssemblyChange();
        void OnSourceFileChange();

    private:
        const ProjectInfo& m_project;
        Set<Ref<AssemblyDefinition>> m_assemblies;
        Map<AssemblyDefinition*, Set<String>> m_assemblyFiles;
        Map<String, AssemblyDefinition*> m_sourceFileAssemblies;
        Map<String, boost::dll::shared_library> m_loadedAssemblies;

    };

}