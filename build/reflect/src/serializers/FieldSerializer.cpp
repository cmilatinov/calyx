#include "serializers/FieldSerializer.h"
#include "serializers/ClassSerializer.h"

namespace reflect::tooling {

    FieldSerializer::FieldSerializer(ClassSerializer& context)
        : m_context(context) {}

    void FieldSerializer::Accept(NamedDecl* decl) {
        // Check is class member
        if (!decl->isCXXClassMember() || decl->getDeclContext() != m_context.GetClassRecord())
            return;

        // Check decl kind is field (instance), or var (static)
        auto declKind = decl->getKind();
        auto printingPolicy = decl->getASTContext().getPrintingPolicy();
        std::string type;
        std::string storageType;
        std::string offset;
        if (declKind == Decl::Kind::Field) {
            auto fieldDecl = ((FieldDecl*) decl);
            storageType = utils::STORAGE_TYPE_INSTANCE;
            type = fieldDecl->getType().getAsString(printingPolicy);
            offset = "offsetof("
                     + fieldDecl->getParent()->getQualifiedNameAsString()
                     + ", " + fieldDecl->getNameAsString() + ")";
        } else if (declKind == Decl::Kind::Var) {
            storageType = utils::STORAGE_TYPE_STATIC;
            type = ((VarDecl*) decl)->getType().getAsString(printingPolicy);
            offset = "(size_t)&" + decl->getQualifiedNameAsString();
        } else {
            return;
        }

        // Add field
        Field field;
        field.access = decl->getAccess();
        field.storageType = storageType;
        field.name = decl->getNameAsString();
        field.fullName = decl->getQualifiedNameAsString();
        field.type = type;
        field.offset = offset;
        m_fields.push_back(field);
    }

    json FieldSerializer::Serialize() {
        json arr = json::array();
        std::transform(m_fields.begin(), m_fields.end(), std::back_inserter(arr), [](const Field& f) {
            return json::object(
                {
                    { "name", f.name },
                    { "fullName", f.fullName },
                    { "type", f.type },
                    { "access", utils::AccessToString(f.access) },
                    { "storageType", f.storageType },
                    { "offset", f.offset }
                }
            );
        });
        return arr;
    }

}