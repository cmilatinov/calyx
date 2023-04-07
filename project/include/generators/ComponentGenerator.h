#pragma once

#include "generators/Generator.h"

namespace Calyx::Editor {

    class ComponentGenerator : public IGenerator {

    public:
        ComponentGenerator(
            const Path& assetDirectory,
            const Path& directory,
            const String& name,
            const String& nspace = "Calyx"
        );

        bool Generate() override;

    private:
        Path m_targetDirectory;
        Path m_projectAssetDirectory;
        String m_componentName;
        String m_componentNamespace;

    };

}