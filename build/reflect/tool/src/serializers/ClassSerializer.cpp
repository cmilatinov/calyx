#include "serializers/ClassSerializer.h"
#include "serializers/HeaderSerializer.h"

namespace Calyx::Reflect::Tooling {

    ClassSerializer::ClassSerializer(HeaderSerializer& context)
        : m_context(context),
          m_constructorSerializer(*this),
          m_fieldSerializer(*this),
          m_methodSerializer(*this),
          m_reflectedBases() {}

    void ClassSerializer::Accept(CXXRecordDecl* decl) {
        m_record = decl;
        CollectReflectedBases(decl);
        m_constructorSerializer.TraverseDecl(decl);
        m_fieldSerializer.TraverseDecl(decl);
        m_methodSerializer.TraverseDecl(decl);
    }

    json ClassSerializer::Serialize() {
        json bases = json::array();
        std::transform(
            m_reflectedBases.begin(), m_reflectedBases.end(), std::back_inserter(bases),
            [](CXXRecordDecl* base) {
                return base->getQualifiedNameAsString();
            }
        );
        return json::object(
            {
                { "name", m_record->getNameAsString() },
                { "fullName", m_record->getQualifiedNameAsString() },
                { "bases", bases },
                { "constructors", m_constructorSerializer.Serialize() },
                { "fields", m_fieldSerializer.Serialize() },
                { "methods", m_methodSerializer.Serialize() }
            }
        );
    }

    void ClassSerializer::CollectReflectedBases(CXXRecordDecl* decl) {
        for (auto base: decl->bases()) {
            auto* record = base.getType()->getAsCXXRecordDecl();
            if (Utils::IsReflected(record)) {
                m_reflectedBases.push_back(record);
            }
            CollectReflectedBases(record);
        }
    }

}