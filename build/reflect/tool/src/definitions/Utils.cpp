#include "definitions/Utils.h"

namespace Calyx::Reflect::Tooling::Utils {

    static DeclAttributesMap s_declAttributes;

    static std::string ConvertValue(const std::string& value) {
        static const auto strRegex = std::regex("\"([^\"]*?)\"");
        std::smatch match;
        if (std::regex_search(value, match, strRegex))
            return match[1];
        return value;
    }

    static Attributes& GetDeclAttributes(const Decl* decl) {
        if (!s_declAttributes.contains(decl)) {
            auto& attributes = s_declAttributes[decl];
            for (const auto* attr: decl->specific_attrs<AnnotateAttr>()) {
                for (const auto& annotation: StringUtils::Split(attr->getAnnotation().str(), ",\\s*")) {
                    auto parts = StringUtils::Split(annotation, "=");
                    auto& key = parts[0];
                    std::string value = (parts.size() == 2) ? ConvertValue(parts[1]) : "true";
                    attributes[key] = value;
                }
            }
        }
        return s_declAttributes[decl];
    }

    bool HasAttribute(
        const Decl* decl,
        const std::string& key
    ) {
        const auto& attributes = GetDeclAttributes(decl);
        return attributes.contains(key);
    }

    bool GetAttributeValue(const Decl* decl, const std::string& key, std::string& value) {
        const auto& attributes = GetDeclAttributes(decl);
        CX_MAP_FIND(attributes, key, it) {
            value = it->second;
            return true;
        }
        return false;
    }

    bool IsReflected(const CXXRecordDecl* classDecl) {
        return classDecl->hasFriends() && std::any_of(
            classDecl->friend_begin(), classDecl->friend_end(),
            [](FriendDecl* fr) {
                return fr->getFriendDecl() != nullptr &&
                       fr->getFriendDecl()->isFunctionOrFunctionTemplate() &&
                       fr->getFriendDecl()->getName().str() == CX_XSTR(CX_REFLECT_REGISTRATION_FN_NAME);
            }
        );
    }

    void CollectRefConversions(const CXXRecordDecl* decl, std::vector<std::string>& outRefs) {
        for (auto* fr: decl->friends()) {
            auto* friendDecl = fr->getFriendDecl();
            if (friendDecl != nullptr &&
                friendDecl->isFunctionOrFunctionTemplate() &&
                friendDecl->getName().str() == CX_XSTR(CX_REFLECT_CONVERTIBLE_REF_FN_NAME)) {
                auto* args = friendDecl->getAsFunction()->getTemplateSpecializationArgs();
                if (args->size() > 0) {
                    auto* otherDecl = args->get(0).getAsType()->getAsCXXRecordDecl();
                    if (otherDecl != nullptr) {
                        outRefs.push_back(otherDecl->getQualifiedNameAsString());
                    }
                }
            }
        }
    }

}