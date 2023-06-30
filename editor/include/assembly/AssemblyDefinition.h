#pragma once

#include "assets/AssetRegistry.h"
#include "assembly/AssemblyDefinitions.h"

namespace Calyx::Editor {

    class AssemblyDefinition : public Asset<AssemblyDefinition> {
        CX_ASSET_REGISTRY_FRIEND();
        CX_BEGIN_REFLECT();

    public:
        explicit AssemblyDefinition(const String& file);
        AssemblyDefinition(const String& name, const Path& directory);

        bool Load(const String& file) override;

        const String& GetName() const { return m_name; }
        const Path& GetDirectory() const { return m_directory; }

        String GetBinaryName() const;

    private:
        Path m_directory;
        String m_file;
        String m_name;

        static AssemblyDefinition* Create(const String& file);

    };

}