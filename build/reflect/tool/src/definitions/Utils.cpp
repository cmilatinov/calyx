#include "definitions/Utils.h"

namespace Calyx::Reflect::Tooling::Utils {

    bool HasAttribute(
        Decl* decl,
        const std::string& key
    ) {
        auto attrRange = decl->specific_attrs<AnnotateAttr>();
        return
            std::any_of(
                attrRange.begin(), attrRange.end(),
                [&key](auto attr) {
                    return attr->getAnnotation().str() == key;
                }
            );
    }

    bool AttributeValue(Decl* decl, const std::string& key, std::string& value) {
        for (auto attr: decl->specific_attrs<AnnotateAttr>()) {
            auto annotation = attr->getAnnotation().str();
            size_t valueIndex;
            if (annotation.rfind(key, 0) == 0 && (valueIndex = annotation.find('=')) != std::string::npos) {
                value = annotation.substr(valueIndex);
                return true;
            }
        }
        return false;
    }

    bool IsReflected(CXXRecordDecl* classDecl) {
        return classDecl->hasFriends() && std::any_of(
            classDecl->friend_begin(), classDecl->friend_end(),
            [](FriendDecl* fr) {
                return fr->getFriendDecl() != nullptr &&
                       fr->getFriendDecl()->isFunctionOrFunctionTemplate() &&
                       fr->getFriendDecl()->getName().str() == CX_XSTR(CX_REFLECT_REGISTRATION_FN_NAME);
            }
        );
    }

}