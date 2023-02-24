#pragma once

namespace Calyx::Reflect::Tooling {

    class ClassSerializer;

    class FieldSerializer :
        public IDeclProcessor<FieldDecl>,
        public IJSONSerializer,
        public RecursiveASTVisitor<FieldSerializer> {

    public:
        explicit FieldSerializer(ClassSerializer& context);

        bool VisitFieldDecl(FieldDecl* decl) {
            Accept(decl);
            return true;
        }

        void Accept(FieldDecl* decl) override;
        json Serialize() override;

        const std::vector<Field>& GetFields() const { return m_fields; }

    private:
        ClassSerializer& m_context;
        std::vector<Field> m_fields;

    };

}