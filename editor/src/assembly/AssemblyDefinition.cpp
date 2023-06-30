#include "assembly/AssemblyDefinition.h"
#include "project/ProjectManager.h"

namespace Calyx::Editor {

    AssemblyDefinition::AssemblyDefinition(const String& file) {
        AssemblyDefinition::Load(file);
    }

    AssemblyDefinition::AssemblyDefinition(const String& name, const Path& directory)
        : m_name(name), m_directory(directory) {}

    bool AssemblyDefinition::Load(const String& file) {
        auto path = Path(file);
        std::ifstream stream(path);
        if (!stream.is_open()) return false;
        json def = json::parse(stream);
        m_name = def[CX_ASSEMBLY_NAME].get<String>();
        m_directory = path.parent_path();
        return true;
    }

    String AssemblyDefinition::GetBinaryName() const {
#ifdef CX_PLATFORM_LINUX
        return fmt::format("libAssembly{}.so", m_name);
#elif defined(CX_PLATFORM_WINDOWS)
        return fmt::format("Assembly{}.dll", m_name);
#else
        return fmt::format("Assembly{}", m_name);
#endif
    }

    AssemblyDefinition* AssemblyDefinition::Create(const String& file) {
        return new AssemblyDefinition(file);
    }

}