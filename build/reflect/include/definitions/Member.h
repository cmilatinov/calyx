#pragma once

namespace reflect::tooling {

    struct Member {
        std::string name;
        std::string fullName;
        AccessSpecifier access;
        std::string storageType;
    };

    struct Field : public Member {
        std::string type;
        std::string offset;
    };

    struct Parameter {
        std::string name;
        std::string type;
    };

    struct Method : public Member {
        std::string pointerType;
        std::vector<Parameter> parameters;
        std::string returnType;
        std::string invocation;
    };

}