#include "serializers/ConstructorSerializer.h"
#include "serializers/ClassSerializer.h"

namespace Calyx::Reflect::Tooling {

    ConstructorSerializer::ConstructorSerializer(ClassSerializer& context)
        : m_context(context) {}

    void ConstructorSerializer::Accept(CXXConstructorDecl* decl) {
        // Check is class member
        if (decl->getParent() != m_context.GetClassRecord())
            return;

        // Add constructor
        auto printingPolicy = decl->getASTContext().getPrintingPolicy();
        Constructor constructor;
        constructor.name = decl->getNameAsString();
        constructor.fullName = decl->getQualifiedNameAsString();
        constructor.pointer = "&" + constructor.fullName;
        int numParams = decl->getNumParams();
        constructor.parameterTypes.resize(numParams);
        for (int i = 0; i < numParams; i++) {
            constructor.parameterTypes[i] = decl->getParamDecl(i)->getType().getAsString(printingPolicy);
        }
        m_constructors.push_back(constructor);
    }

    json ConstructorSerializer::Serialize() {
        json arr = json::array();
        std::transform(
            m_constructors.begin(), m_constructors.end(), std::back_inserter(arr), [](const Constructor& c) {
                return json::object(
                    {
                        { "name", c.name },
                        { "fullName", c.fullName },
                        { "pointer", c.pointer },
                        { "parameters", c.parameterTypes }
                    }
                );
            }
        );
        return arr;
    }

}