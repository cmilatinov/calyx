#pragma once

#include "project/ProjectDefinitions.h"

namespace Calyx::Editor {

    class ProjectGenerator {

    public:
        ProjectGenerator(const String& directory, const String& fileName, const String& name);

        bool Generate();

    private:
        Path m_projectDirectory;
        Path m_projectFile;
        String m_projectName;

    };

}