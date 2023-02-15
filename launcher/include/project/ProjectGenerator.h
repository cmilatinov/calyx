#pragma once

namespace Calyx::Launcher {

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