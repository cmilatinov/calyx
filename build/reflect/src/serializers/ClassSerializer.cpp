#include "serializers/ClassSerializer.h"
#include "serializers/HeaderSerializer.h"

namespace reflect::tooling {

    ClassSerializer::ClassSerializer(HeaderSerializer& context)
        : m_context(context), m_fieldSerializer(*this), m_methodSerializer(*this) {}

    void ClassSerializer::Accept(CXXRecordDecl* decl) {
        m_record = decl;

        // TODO: Collect base class fields recursively
        for (auto base: decl->bases()) {
            m_context.GetSerializer(base.getType()->getAsCXXRecordDecl());
        }

        m_fieldSerializer.TraverseDecl(decl);
        m_methodSerializer.TraverseDecl(decl);
    }

    json ClassSerializer::Serialize() {
        json bases = json::array();
        std::transform(m_record->bases().begin(), m_record->bases().end(), std::back_inserter(bases),
                       [](const CXXBaseSpecifier& base) {
                            return base.getType().getTypePtr()->getAsCXXRecordDecl()->getNameAsString();
                       }
        );
        return json::object(
            {
                { "name", m_record->getNameAsString() },
                { "fullName", m_record->getQualifiedNameAsString() },
                { "namespace", m_record->getParent()->getDeclKindName() },
                { "bases", bases },
                { "fields", m_fieldSerializer.Serialize() },
                { "methods", m_methodSerializer.Serialize() }
            }
        );
    }

    void ClassSerializer::CollectAccessibleMembers() {
        m_fieldSerializer.GetFields();
    }

}