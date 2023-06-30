#include "generators/AssemblyGenerator.h"
#include "assembly/AssemblyDefinitions.h"

namespace Calyx::Editor {

    AssemblyGenerator::AssemblyGenerator(const String& name, const String& file)
        : m_name(name), m_file(file) {}

    bool AssemblyGenerator::Generate() {
        std::ofstream file(m_file);
        if (!file.is_open()) return false;
        json assembly = json::object(
            {
                { CX_ASSEMBLY_NAME, m_name },
                { CX_ASSEMBLY_SOURCES, m_sources },
                { CX_ASSEMBLY_HEADERS, m_headers }
            }
        );
        file << std::setw(4) << assembly;
        return true;
    }

}

