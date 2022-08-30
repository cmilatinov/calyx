#include "definitions/Utils.h"

namespace reflect::tooling::utils {

    std::string AccessToString(AccessSpecifier access) {
        switch (access) {
            case AS_public:
                return "AccessSpecifier::PUBLIC";
            case AS_protected:
                return "AccessSpecifier::PROTECTED";
            case AS_private:
                return "AccessSpecifier::PRIVATE";
        }
        return "none";
    }

    bool ReplaceInString(std::string& str, const std::string& occurrence, const std::string& replacement) {
        size_t startPos = str.find(occurrence);
        if (startPos == std::string::npos)
            return false;

        str.replace(startPos, occurrence.length(), replacement);
        return true;
    }

}