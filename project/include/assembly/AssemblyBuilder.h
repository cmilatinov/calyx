#pragma once

#include "project/ProjectInfo.h"

namespace Calyx::Editor {

    class AssemblyBuilder {

    public:
        AssemblyBuilder(const ProjectInfo& projectInfo);

        void BuildAssemblies();

    private:
        void GenerateAssembliesCMake();
        void InvokeBuildSystem();

    private:
        const ProjectInfo& m_project;

    };

}