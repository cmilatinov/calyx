#include "generators/ComponentGenerator.h"

#include "templates/ComponentHeader.h"
#include "templates/ComponentSource.h"

#include <inja/inja.hpp>

namespace Calyx::Editor {

    ComponentGenerator::ComponentGenerator(
        const Path& assetDirectory,
        const Path& directory,
        const String& name,
        const String& nspace
    ) : m_projectAssetDirectory(assetDirectory),
        m_targetDirectory(directory),
        m_componentName(name),
        m_componentNamespace(nspace) {}

    bool ComponentGenerator::Generate() {
        inja::Environment env;
        inja::Template headerTemplate = env.parse(TEMPLATE_COMPONENT_HEADER);
        inja::Template sourceTemplate = env.parse(TEMPLATE_COMPONENT_SOURCE);

        Path headerPath = m_targetDirectory / (m_componentName + ".h");
        Path sourcePath = m_targetDirectory / (m_componentName + ".cpp");

        json data = json::object(
            {
                { "name", m_componentName },
                { "namespace", m_componentNamespace },
                { "header", FileSystem::relative(headerPath, m_projectAssetDirectory).string() }
            }
        );

        std::ofstream header(headerPath);
        if (!header.is_open()) return false;
        env.render_to(header, headerTemplate, data);

        std::ofstream source(sourcePath);
        if (!source.is_open()) return false;
        env.render_to(source, sourceTemplate, data);

        return true;
    }

}