#include "serializers/HeaderSerializer.h"

namespace Calyx::Reflect::Tooling {

    HeaderSerializer::HeaderSerializer(std::string headerFile)
        : m_headerFile(std::move(headerFile)) {}

    void HeaderSerializer::run(const MatchFinder::MatchResult& Result) {
        auto* classDecl = const_cast<CXXRecordDecl*>(Result.Nodes.getNodeAs<CXXRecordDecl>("classes"));
        auto location = classDecl->getLocation();
        auto& srcManager = Result.Context->getSourceManager();
        location = srcManager.getFileLoc(location);
        auto name = classDecl->getNameAsString();
        auto filePath = srcManager.getFilename(location).str();
        auto headerPath = std::filesystem::absolute(m_headerFile).string();
        if (filePath != headerPath)
            return;

        auto& classSerializer = m_classSerializers[classDecl] = std::make_unique<ClassSerializer>(*this);
        classSerializer->Accept(classDecl);
        m_jsonObject.push_back(classSerializer->Serialize());
        std::cout << classDecl->getQualifiedNameAsString() << std::endl;
    }

    const json& HeaderSerializer::GetJSONObject() {
        return m_jsonObject;
    }

}