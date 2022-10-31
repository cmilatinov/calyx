#include "serializers/FieldSerializer.h"
#include "serializers/ClassSerializer.h"

namespace Calyx::Reflect::Tooling {

    FieldSerializer::FieldSerializer(ClassSerializer& context)
        : m_context(context) {}

    void FieldSerializer::Accept(FieldDecl* decl) {
        // Check is class member
        if (!decl->isCXXClassMember() || decl->getDeclContext() != m_context.GetClassRecord())
            return;

        // Check member is public or serialized
        if (decl->getAccess() != clang::AS_public && !Utils::HasAttribute(decl, CX_REFLECT_SERIALIZE_ANNOTATION))
            return;

        // Add field
        auto printingPolicy = decl->getASTContext().getPrintingPolicy();
        Field field;
        field.type = decl->getType().getAsString(printingPolicy);
        field.name = decl->getNameAsString();
        field.fullName = decl->getQualifiedNameAsString();
        field.pointer = "&" + decl->getQualifiedNameAsString();
        m_fields.push_back(field);
    }

    json FieldSerializer::Serialize() {
        json arr = json::array();
        std::transform(
            m_fields.begin(), m_fields.end(), std::back_inserter(arr), [](const Field& f) {
                return json::object(
                    {
                        { "type", f.type },
                        { "name", f.name },
                        { "fullName", f.fullName },
                        { "pointer", f.pointer }
                    }
                );
            }
        );
        return arr;
    }

}