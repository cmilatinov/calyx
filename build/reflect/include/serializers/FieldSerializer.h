#pragma once

namespace reflect::tooling {

    class ClassSerializer;

    class FieldSerializer :
        public IDeclProcessor<NamedDecl>,
        public IJSONSerializer,
        public RecursiveASTVisitor<FieldSerializer> {

    public:
        explicit FieldSerializer(ClassSerializer& context);

        bool VisitNamedDecl(NamedDecl* decl) {
            Accept(decl);
            return true;
        }

        void Accept(NamedDecl* decl) override;
        json Serialize() override;

        const std::vector<Field>& GetFields() const { return m_fields; }

    private:
        ClassSerializer& m_context;
        std::vector<Field> m_fields;

    };

}