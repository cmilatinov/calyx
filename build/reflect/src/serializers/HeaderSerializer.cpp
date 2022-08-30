#include "serializers/HeaderSerializer.h"

namespace reflect::tooling {

    HeaderSerializer::~HeaderSerializer() {
        for (const auto& cs: m_classSerializers) {
            delete cs.second;
        }
    }

    void HeaderSerializer::run(const MatchFinder::MatchResult& Result) {
        auto* classDecl = const_cast<CXXRecordDecl*>(Result.Nodes.getNodeAs<CXXRecordDecl>("classes"));
        auto* classSerializer = new ClassSerializer(*this);
        m_classSerializers[classDecl] = classSerializer;
        classSerializer->Accept(classDecl);
        m_jsonObject.push_back(classSerializer->Serialize());
    }

    ClassSerializer* HeaderSerializer::GetSerializer(CXXRecordDecl* decl) {
        if (m_classSerializers.count(decl) == 0)
            return nullptr;
        return m_classSerializers[decl];
    }

    const json& HeaderSerializer::GetJSONObject() {
        return m_jsonObject;
    }

}