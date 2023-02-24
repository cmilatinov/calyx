#pragma once

namespace Calyx::Reflect::Tooling::Utils {

    using Attributes = std::unordered_map<std::string, std::string>;
    using DeclAttributesMap = std::unordered_map<const Decl*, Attributes>;

    bool HasAttribute(const Decl* decl, const std::string& key);
    bool GetAttributeValue(const Decl* decl, const std::string& key, std::string& value);
    bool IsReflected(const CXXRecordDecl* classDecl);
    void CollectRefConversions(const CXXRecordDecl* decl, std::vector<std::string>& outRefs);

}