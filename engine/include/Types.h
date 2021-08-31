#pragma once

#ifdef CX_DEBUG
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>

#include <glm/glm.hpp>
#include <spdlog/spdlog.h>

namespace Calyx {

    using String = std::string;

    template<typename T>
    using Array = std::vector<T>;

    template<typename K, typename V>
    using UMap = std::unordered_map<K, V>;

    template<typename K, typename V>
    using Map = std::map<K, V>;

    template<typename T>
    using USet = std::unordered_set<T>;

    template<typename T>
    using Set = std::set<T>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;

    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;

    using vec2 = glm::vec2;
    using vec3 = glm::vec3;
    using vec4 = glm::vec4;

    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;

    using spdlog::debug;
    using spdlog::info;
    using spdlog::warn;
    using spdlog::error;

}

using namespace Calyx;