#pragma once

#include <compare>
#include <chrono>

namespace Calyx::ChronoUtils {

    template<typename Clock, typename Duration>
    int Compare(
        const std::chrono::time_point <Clock, Duration>& first,
        const std::chrono::time_point <Clock, Duration>& second
    ) {
        std::partial_ordering cmp = first <=> second;
        if (cmp == std::partial_ordering::less)
            return -1;
        if (cmp == std::partial_ordering::greater)
            return 1;
        return 0;
    }

}