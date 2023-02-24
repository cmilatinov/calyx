#include "assets/Assembly.h"

namespace Calyx {

    void Assembly::WriteFile() {
        std::ofstream file(m_file);
        if (!file.is_open()) return;
        file << std::setw(4) << ToJSON();
    }

    json Assembly::ToJSON() {
        return json::object({
            { CX_ASSEMBLY_NAME, m_name },
            { CX_ASSEMBLY_SOURCES, m_sources },
            { CX_ASSEMBLY_SOURCE_FILES, m_sourceFiles }
        });
    }

    Assembly* Assembly::Create(const String& file) {
        std::ifstream stream(file);
        if (!stream.is_open())
            return nullptr;

        json assembly = json::parse(stream);
        auto* instance = new Assembly();
        instance->m_file = file;
        instance->m_name = assembly[CX_ASSEMBLY_NAME].get<String>();
        instance->m_sources = assembly[CX_ASSEMBLY_SOURCES].get<List<String>>();
        auto sourceFiles = assembly[CX_ASSEMBLY_SOURCE_FILES].get<List<String>>();
        for (const auto& sourceFile: sourceFiles) {
            instance->m_sourceFiles.insert(sourceFile);
        }
        return instance;
    }

    Assembly Assembly::CreateDefaultAssembly(const String& file) {
        Assembly result;
        result.m_file = file;
        result.m_name = "Default";
        result.m_sources.emplace_back("**.cpp");
        return result;
    }

}