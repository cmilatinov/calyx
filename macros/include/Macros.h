#pragma once

#define CX_BIT(x) (1 << x)

#define CX_XSTR(x) CX_STR(x)
#define CX_STR(x) #x

#define CX_CONCAT(a, b) CX_CONCAT_IMPL(a, b)
#define CX_CONCAT_IMPL(a, b) a##b

#define CX_BIND_EVENT_METHOD(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define CX_DISPATCH_EVENT(type, fn, event) event.Dispatch<type>(CX_BIND_EVENT_METHOD(fn))

#define CX_SINGLETON(name)                      \
private:                                        \
static std::unique_ptr<name> s_instance;        \
                                                \
public:                                         \
static inline void Init() {                     \
    if (s_instance == nullptr)                  \
        s_instance = std::make_unique<name>();  \
}                                               \
                                                \
static inline name& GetInstance() {             \
    return *s_instance.get();                   \
}                                               \
                                                \
name(name& other) = delete;                     \
void operator=(const name& other) = delete

#define CX_SINGLETON_INSTANCE(name) std::unique_ptr<name> name::s_instance = nullptr

#define CX_SINGLETON_EXPOSE_METHOD(instanceName, fn, ...)  \
static inline fn {                                         \
    return s_instance->instanceName(__VA_ARGS__);          \
}

#define CX_MAP_FIND(map, key, it) if (auto it = map.find(key); it != map.end())