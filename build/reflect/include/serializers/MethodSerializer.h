#pragma once

namespace reflect::tooling {

    class ClassSerializer;

    class MethodSerializer :
        public IDeclProcessor<CXXMethodDecl>,
        public IJSONSerializer,
        public RecursiveASTVisitor<MethodSerializer> {

    public:
        explicit MethodSerializer(ClassSerializer& context);

        bool VisitCXXMethodDecl(CXXMethodDecl* decl) {
            Accept(decl);
            return true;
        }

        void Accept(CXXMethodDecl* decl) override;
        json Serialize() override;

    private:
        ClassSerializer& m_context;
        std::vector<Method> m_methods;

    };

}