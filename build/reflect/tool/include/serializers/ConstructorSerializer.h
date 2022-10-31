#pragma once

namespace Calyx::Reflect::Tooling {

    class ClassSerializer;

    class ConstructorSerializer :
        public IDeclProcessor<CXXConstructorDecl>,
        public IJSONSerializer,
        public RecursiveASTVisitor<ConstructorSerializer> {

    public:
        explicit ConstructorSerializer(ClassSerializer& context);

        bool VisitCXXConstructorDecl(CXXConstructorDecl* decl) {
            Accept(decl);
            return true;
        }

        void Accept(CXXConstructorDecl* decl) override;
        json Serialize() override;

    private:
        ClassSerializer& m_context;
        std::vector<Constructor> m_constructors;

    };

}