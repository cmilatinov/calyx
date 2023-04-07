#include "ValidationUtils.h"

namespace Calyx::ValidationUtils {

    bool ValidateRequired(const std::string& value) {
        return !value.empty();
    }

    bool ValidateAlphaOnly(const std::string& value) {
        std::regex rgx("[^a-zA-Z]");
        return !std::regex_match(value, rgx);
    }

}