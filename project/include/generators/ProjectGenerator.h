#pragma once

#include "generators/Generator.h"
#include "project/ProjectDefinitions.h"

namespace Calyx::Editor {

    class ProjectGenerator : public IGenerator {

    public:
        ProjectGenerator(const String& directory, const String& fileName, const String& name);

        bool Generate() override;

    private:
        Path m_projectDirectory;
        Path m_projectFile;
        String m_projectName;

    };

}