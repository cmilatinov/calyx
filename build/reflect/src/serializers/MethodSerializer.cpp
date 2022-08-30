#include "serializers/MethodSerializer.h"
#include "serializers/ClassSerializer.h"

namespace reflect::tooling {

    MethodSerializer::MethodSerializer(ClassSerializer& context)
        : m_context(context) {}

    void MethodSerializer::Accept(CXXMethodDecl* decl) {
        // Check is class member
        if (decl->getParent() != m_context.GetClassRecord())
            return;

        // Add method
        auto printingPolicy = decl->getASTContext().getPrintingPolicy();
        Method method;
        method.access = decl->getAccess();
        method.storageType = decl->isStatic() ?
                             utils::STORAGE_TYPE_STATIC :
                             utils::STORAGE_TYPE_INSTANCE;
        method.name = decl->getNameAsString();
        method.fullName = decl->getQualifiedNameAsString();
        method.parameters.resize(decl->getNumParams());
        for (int i = 0; i < decl->getNumParams(); i++) {
            method.parameters[i].name = decl->getParamDecl(i)->getNameAsString();
            method.parameters[i].type = decl->getParamDecl(i)->getType().getAsString(printingPolicy);
        }
        method.returnType = decl->getReturnType().getAsString(printingPolicy);
        method.pointerType = decl->isStatic() ?
                             decl->getASTContext()
                                 .getPointerType(decl->getType())
                                 .getAsString(printingPolicy) :
                             decl->getASTContext()
                                 .getMemberPointerType(decl->getType(),
                                                       decl->getParent()->getTypeForDecl())
                                 .getAsString(printingPolicy);
        std::string paramList = utils::Join<Parameter>(
            ", ",
            method.parameters,
            [i = 0](const Parameter& p) mutable {
                std::string str = "args[" +
                                  std::to_string(i) +
                                  "].GetT<std::decay_t<" +
                                  p.type +
                                  ">>()";
                i++;
                return str;
            }
        );
        method.invocation = decl->isStatic() ?
            method.fullName + "(" + paramList + ")" :
            "o.GetT<" + decl->getParent()->getQualifiedNameAsString() +
            ">()." + method.name + "(" + paramList + ")";
        m_methods.push_back(method);
    }

    json MethodSerializer::Serialize() {
        json arr = json::array();
        std::transform(m_methods.begin(), m_methods.end(), std::back_inserter(arr), [](const Method& m) {
            json params = json::array();
            std::transform(m.parameters.begin(), m.parameters.end(), std::back_inserter(params),
                           [](const Parameter& p) {
                               return json::object(
                                   {
                                       { "name", p.name },
                                       { "type", p.type }
                                   }
                               );
                           });
            return json::object(
                {
                    { "name", m.name },
                    { "fullName", m.fullName },
                    { "pointerType", m.pointerType },
                    { "access", utils::AccessToString(m.access) },
                    { "storageType", m.storageType },
                    { "isStatic", m.storageType == utils::STORAGE_TYPE_STATIC },
                    { "isVoidReturn", m.returnType == "void" },
                    { "invocation", m.invocation },
                    { "parameters", params },
                    { "returnType", m.returnType }
                }
            );
        });
        return arr;
    }

}