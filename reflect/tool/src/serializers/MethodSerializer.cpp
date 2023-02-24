#include "serializers/MethodSerializer.h"
#include "serializers/ClassSerializer.h"

namespace Calyx::Reflect::Tooling {

    MethodSerializer::MethodSerializer(ClassSerializer& context)
        : m_context(context) {}

    void MethodSerializer::Accept(CXXMethodDecl* decl) {
        // Check is class member
        if (decl->getParent() != m_context.GetClassRecord())
            return;

        // Remove constructors and destructors + No static methods
        if (decl->getKind() != clang::Decl::Kind::CXXMethod || decl->isStatic())
            return;

        // Add method
        auto printingPolicy = decl->getASTContext().getPrintingPolicy();
        Method method;
        method.name = decl->getNameAsString();
        method.fullName = decl->getQualifiedNameAsString();
        method.pointer = "&" + method.fullName;
        method.type = decl->getType().getAsString(printingPolicy);
        m_methods.push_back(method);
    }

    json MethodSerializer::Serialize() {
        json arr = json::array();
        std::transform(
            m_methods.begin(), m_methods.end(), std::back_inserter(arr), [](const Method& m) {
                return json::object(
                    {
                        { "type", m.type },
                        { "name", m.name },
                        { "fullName", m.fullName },
                        { "pointer", m.pointer }
                    }
                );
            }
        );
        return arr;
    }

}