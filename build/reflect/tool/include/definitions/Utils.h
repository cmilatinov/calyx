#pragma once

namespace Calyx::Reflect::Tooling::Utils {

    bool HasAttribute(Decl* decl, const std::string& key);
    bool AttributeValue(Decl* decl, const std::string& key, std::string& value);
    bool IsReflected(CXXRecordDecl* classDecl);
    void CollectRefConversions(CXXRecordDecl* decl, std::vector<std::string>& outRefs);

}