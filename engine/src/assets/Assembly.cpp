#include "assets/Assembly.h"

namespace Calyx {

    void Assembly::WriteFile() {
        std::ofstream file(m_file);
        if (!file.is_open()) return;
        file << std::setw(4) << ToJSON();
    }

    json Assembly::ToJSON() {
        return json::object(
            {
                { CX_ASSEMBLY_NAME, m_name },
                { CX_ASSEMBLY_SOURCES, m_sources },
                { CX_ASSEMBLY_SOURCE_FILES, m_sourceFiles },
                { CX_ASSEMBLY_SOURCE_HEADERS, m_sourceHeaders }
            }
        );
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

    void Assembly::UpdateSourceHeaders(const Path& projectFolder) {
        Set<String> headers;
        m_sourceHeaders.clear();
        std::transform(
            m_sourceFiles.begin(), m_sourceFiles.end(),
            std::inserter(headers, headers.end()),
            [](const auto& file) {
                Path path(file);
                path = path.parent_path() / (path.stem().string() + ".h");
                return path.string();
            }
        );
        std::copy_if(
            headers.begin(), headers.end(),
            std::inserter(m_sourceHeaders, m_sourceHeaders.begin()),
            [&projectFolder](const auto& header) { return FileSystem::is_regular_file(projectFolder / header); }
        );
    }

    String Assembly::GetBinaryName() const {
#ifdef CX_PLATFORM_LINUX
        return fmt::format("libAssembly{}.so", m_name);
#elif defined(CX_PLATFORM_WINDOWS)
        return fmt::format("Assembly{}.dll", m_name);
#else
        return fmt::format("Assembly{}", m_name);
#endif
    }

    void Assembly::SetSourceFiles(const Path& projectFolder, const Set<String>& sourceFiles) {
        m_sourceFiles = sourceFiles;
        UpdateSourceHeaders(projectFolder);
    }

}