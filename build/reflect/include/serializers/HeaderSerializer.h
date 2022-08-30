#pragma once

#include "serializers/ClassSerializer.h"

namespace reflect::tooling {

    class HeaderSerializer : public MatchFinder::MatchCallback {

    public:
        ~HeaderSerializer() override;

        void run(const MatchFinder::MatchResult& Result) override;

        ClassSerializer* GetSerializer(CXXRecordDecl* decl);

        const json& GetJSONObject();

    private:
        std::unordered_map<CXXRecordDecl*, ClassSerializer*> m_classSerializers;
        json m_jsonObject = json::array();

    };

}

