#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <variant>
#include <type_traits>
#include <numeric>
#include <typeinfo>

#include <string>
#include <sstream>
#include <fstream>
#include <array>
#include <vector>
#include <list>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <thread>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <nameof.hpp>
#include <stduuid/uuid.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <nlohmann/json.hpp>

#include "Macros.h"
#include "Reflect.h"

#include "ChronoUtils.h"
#include "StringUtils.h"
#include "UUIDUtils.h"
#include "FormatUtils.h"
#include "PathUtils.h"
#include "ValidationUtils.h"

namespace Calyx {

    using UUID = uuids::uuid;

    using String = std::string;

    using StringView = std::string_view;

    using StringStream = std::stringstream;

    template<typename T>
    using List = std::vector<T>;

    template<typename T>
    using LinkedList = std::list<T>;

    template<typename T>
    using Deque = std::deque<T>;

    template<typename K, typename V>
    using Map = std::unordered_map<K, V>;

    template<typename K, typename V>
    using OrderedMap = std::map<K, V>;

    template<typename T>
    using Set = std::unordered_set<T>;

    template<typename T>
    using OrderedSet = std::set<T>;

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using WeakRef = std::weak_ptr<T>;

    template<typename T>
    using Optional = std::optional<T>;

    template<typename T>
    using Function = std::function<T>;

    namespace FileSystem = std::filesystem;
    using Path = std::filesystem::path;

    using IdentifierType = entt::id_type;

    namespace ThisThread = std::this_thread;
    using Thread = std::thread;

    namespace Integer {
        using uint8 = uint8_t;
        using uint16 = uint16_t;
        using uint32 = uint32_t;
        using uint64 = uint64_t;
        typedef unsigned __int128 uint128;

        using int8 = int8_t;
        using int16 = int16_t;
        using int32 = int32_t;
        using int64 = int64_t;
        typedef __int128 int128;
    }

    namespace Math {
        using vec2 = glm::vec2;
        using vec3 = glm::vec3;
        using vec4 = glm::vec4;

        using ivec2 = glm::ivec2;
        using ivec3 = glm::ivec3;
        using ivec4 = glm::ivec4;

        using uvec2 = glm::uvec2;
        using uvec3 = glm::uvec3;
        using uvec4 = glm::uvec4;

        using mat2 = glm::mat2;
        using mat3 = glm::mat3;
        using mat4 = glm::mat4;

        using quat = glm::quat;
    }

    namespace Chrono {
        using namespace std::chrono;
        using Hours = hours;
        using Minutes = minutes;
        using Seconds = seconds;
        using Milliseconds = milliseconds;
        using Microseconds = microseconds;
        using Nanoseconds = nanoseconds;
        using Clock = high_resolution_clock;

        template<class ToDuration, class Rep, class Period>
        constexpr ToDuration DurationCast(const duration<Rep, Period>& d) {
            return duration_cast<ToDuration>(d);
        }
    }

    namespace FileSystem = std::filesystem;

    template<typename T, typename ... Args>
    inline constexpr Scope<T> CreateScope(Args&& ... args) {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename ... Args>
    inline constexpr Ref<T> CreateRef(Args&& ... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

using namespace Calyx;
using namespace Calyx::Integer;
using namespace Calyx::Math;
using namespace Calyx::Chrono;
using namespace Calyx::Reflect;

using nlohmann::json;