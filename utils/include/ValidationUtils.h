#pragma once

#include <string>
#include <regex>

namespace Calyx::ValidationUtils {

    bool ValidateRequired(const std::string& value);

    bool ValidateAlphaOnly(const std::string& value);
    
}