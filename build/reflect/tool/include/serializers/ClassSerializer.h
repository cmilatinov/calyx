#pragma once

#include "serializers/ConstructorSerializer.h"
#include "serializers/FieldSerializer.h"
#include "serializers/MethodSerializer.h"

namespace Calyx::Reflect::Tooling {

    class HeaderSerializer;

    class ClassSerializer :
        public IDeclProcessor<CXXRecordDecl>,
        public IJSONSerializer {

    public:
        explicit ClassSerializer(HeaderSerializer& context);

        void Accept(CXXRecordDecl* decl) override;
        json Serialize() override;

        [[nodiscard]] const FieldSerializer& GetFieldSerializer() const { return m_fieldSerializer; }
        [[nodiscard]] const MethodSerializer& GetMethodSerializer() const { return m_methodSerializer; }

        CXXRecordDecl* GetClassRecord() { return m_record; }

    private:
        CXXRecordDecl* m_record = nullptr;
        HeaderSerializer& m_context;
        ConstructorSerializer m_constructorSerializer;
        FieldSerializer m_fieldSerializer;
        MethodSerializer m_methodSerializer;
        std::vector<CXXRecordDecl*> m_reflectedBases;
        std::vector<std::string> m_refConversions;

        void CollectReflectedBases(CXXRecordDecl* decl);

    };

}

