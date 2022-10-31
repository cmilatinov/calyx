#include "serializers/HeaderSerializer.h"

namespace Calyx::Reflect::Tooling {

    HeaderSerializer::HeaderSerializer(std::string headerFile)
        : m_headerFile(std::move(headerFile)) {}

    HeaderSerializer::~HeaderSerializer() {
        for (const auto& cs: m_classSerializers) {
            delete cs.second;
        }
    }

    void HeaderSerializer::run(const MatchFinder::MatchResult& Result) {
        auto* classDecl = const_cast<CXXRecordDecl*>(Result.Nodes.getNodeAs<CXXRecordDecl>("classes"));
        auto location = classDecl->getLocation();
        auto& scrManager = Result.Context->getSourceManager();
        const auto* fileEntry = scrManager.getFileEntryForID(scrManager.getFileID(location));
        auto filePath = fileEntry->tryGetRealPathName().str();
        auto headerPath = std::filesystem::absolute(m_headerFile).string();
        if (filePath != headerPath)
            return;

        auto* classSerializer = new ClassSerializer(*this);
        m_classSerializers[classDecl] = classSerializer;
        classSerializer->Accept(classDecl);
        m_jsonObject.push_back(classSerializer->Serialize());
        std::cout << classDecl->getQualifiedNameAsString() << std::endl;
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