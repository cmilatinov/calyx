#pragma once

#include "serializers/ClassSerializer.h"

namespace Calyx::Reflect::Tooling {

    class HeaderSerializer : public MatchFinder::MatchCallback {

    public:
        explicit HeaderSerializer(std::string headerFile);

        void run(const MatchFinder::MatchResult& Result) override;

        const json& GetJSONObject();

    private:
        std::string m_headerFile;
        std::unordered_map<CXXRecordDecl*, std::unique_ptr<ClassSerializer>> m_classSerializers;
        json m_jsonObject = json::array();

    };

}

