#pragma once

#include "generators/Generator.h"

namespace Calyx::Editor {

    class AssemblyGenerator : public IGenerator {

    public:
        AssemblyGenerator(const String& name, const String& file);

        void AddSource(const String& source) { m_sources.push_back(source); }
        void AddHeader(const String& header) { m_headers.push_back(header); }

        bool Generate() override;

    private:
        String m_name;
        Path m_file;
        List<String> m_sources;
        List<String> m_headers;

    };

}