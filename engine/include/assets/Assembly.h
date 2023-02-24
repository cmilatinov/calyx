#pragma once

#include "assets/AssetRegistry.h"

#define CX_ASSEMBLY_NAME            "name"
#define CX_ASSEMBLY_SOURCES         "sources"
#define CX_ASSEMBLY_SOURCE_FILES    "source_files"

namespace Calyx {

    class Assembly : public Asset<Assembly> {
        CX_ASSET_REGISTRY_FRIEND();
        CX_BEGIN_REFLECT();

    private:
        Assembly() = default;

    public:
        const String& GetName() const { return m_name; }
        const List<String>& GetSources() const { return m_sources; }
        const Set<String>& GetSourceFiles() const { return m_sourceFiles; }

        void SetSourceFiles(const Set<String>& sourceFiles) { m_sourceFiles = sourceFiles; }

        void WriteFile();

        json ToJSON();

        static Assembly CreateDefaultAssembly(const String& file);

    private:
        String m_file;
        String m_name;
        List<String> m_sources;
        Set<String> m_sourceFiles;

    private:
        static Assembly* Create(const String& file);

    };

}