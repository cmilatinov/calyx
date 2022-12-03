#pragma once

#include <filesystem>

#ifdef CX_ENABLE_ASSERTS
#define CX_INTERNAL_ASSERT_IMPL(type, check, msg, ...) do { if(!(check)) { CX##type##ERROR(msg, __VA_ARGS__); CX_DEBUGBREAK(); } } while(false)
#define CX_INTERNAL_ASSERT_WITH_MSG(type, check, ...) CX_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define CX_INTERNAL_ASSERT_NO_MSG(type, check) CX_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", #check, std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define CX_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define CX_INTERNAL_ASSERT_GET_MACRO(...) CX_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, CX_INTERNAL_ASSERT_WITH_MSG, CX_INTERNAL_ASSERT_NO_MSG)

#define CX_ASSERT(...) CX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__)
#define CX_CORE_ASSERT(...) CX_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__)
#else
#define CX_ASSERT(...)
#define CX_CORE_ASSERT(...)
#endif