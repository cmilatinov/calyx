#pragma once

namespace Calyx::Reflect::Tooling {

    struct Member {
        std::string type;
        std::string name;
        std::string fullName;
        std::string pointer;
    };

    struct Field : public Member {
        std::string displayName;
    };

    struct Method : public Member {
    };

    struct Constructor : public Member {
        std::vector<std::string> parameterTypes;
    };

}