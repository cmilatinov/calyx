#include "ValidationUtils.h"

namespace Calyx::ValidationUtils {

    bool ValidateRequired(const std::string& value) {
        return !value.empty();
    }

}