#pragma once

#include "serializers/FieldSerializer.h"
#include "serializers/MethodSerializer.h"

namespace reflect::tooling {

    class HeaderSerializer;

    struct Class {
        std::string name;
        std::string fullName;
    };

    class ClassSerializer :
        public IDeclProcessor<CXXRecordDecl>,
        public IJSONSerializer,
        public RecursiveASTVisitor<ClassSerializer> {

    public:
        explicit ClassSerializer(HeaderSerializer& context);

        void Accept(CXXRecordDecl* decl) override;
        json Serialize() override;

        void CollectAccessibleMembers();

        [[nodiscard]] const FieldSerializer& GetFieldSerializer() const { return m_fieldSerializer; }
        [[nodiscard]] const MethodSerializer& GetMethodSerializer() const { return m_methodSerializer; }

        const CXXRecordDecl* GetClassRecord() { return m_record; }

    private:
        CXXRecordDecl* m_record;
        HeaderSerializer& m_context;
        FieldSerializer m_fieldSerializer;
        MethodSerializer m_methodSerializer;

    };

}

