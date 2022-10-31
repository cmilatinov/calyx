#pragma once

#include "serializers/ClassSerializer.h"

namespace Calyx::Reflect::Tooling {

    class HeaderSerializer : public MatchFinder::MatchCallback {

    public:
        explicit HeaderSerializer(std::string headerFile);
        ~HeaderSerializer() override;

        void run(const MatchFinder::MatchResult& Result) override;

        ClassSerializer* GetSerializer(CXXRecordDecl* decl);

        const json& GetJSONObject();

    private:
        std::string m_headerFile;
        std::unordered_map<CXXRecordDecl*, ClassSerializer*> m_classSerializers;
        json m_jsonObject = json::array();

    };

}

