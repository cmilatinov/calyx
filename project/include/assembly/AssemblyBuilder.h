#pragma once

#include "project/ProjectInfo.h"

#include <boost/dll/shared_library.hpp>

namespace Calyx::Editor {

    class AssemblyBuilder {

    public:
        explicit AssemblyBuilder(const ProjectInfo& projectInfo);

        void BuildAssemblies();
        void LoadAssemblies();

    private:
        bool UpdateAssemblies();
        void GenerateAssembliesCMake();
        void ConfigureBuildSystem();
        void InvokeBuildSystem();

    private:
        const ProjectInfo& m_project;
        Map<String, boost::dll::shared_library> m_loadedAssemblies;

    };

}