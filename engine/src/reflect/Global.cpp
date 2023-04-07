
#include "assets/Assembly.h"

CX_REFLECT_REGISTRATION {
    using namespace entt::literals;

    entt::meta<UUID>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(UUID).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<UUID>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<UUID>)
        );

    entt::meta<String>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(String).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<String>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<String>)
        );

    entt::meta<vec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec2).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<vec2>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<vec2>)
        );

    entt::meta<vec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec3).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<vec3>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<vec3>)
        );

    entt::meta<vec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec4).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<vec4>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<vec4>)
        );

    entt::meta<ivec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec2).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<ivec2>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<ivec2>)
        );

    entt::meta<ivec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec3).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<ivec3>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<ivec3>)
        );

    entt::meta<ivec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec4).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<ivec4>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<ivec4>)
        );

    entt::meta<uvec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec2).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<uvec2>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<uvec2>)
        );

    entt::meta<uvec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec3).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<uvec3>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<uvec3>)
        );

    entt::meta<uvec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec4).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<uvec4>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<uvec4>)
        );

    entt::meta<mat2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat2).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<mat2>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<mat2>)
        );

    entt::meta<mat3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat3).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<mat3>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<mat3>)
        );

    entt::meta<mat4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat4).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<mat4>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<mat4>)
        );

    entt::meta<quat>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(quat).data()))
        .prop(CX_REFLECT_TYPE_TRAITS,
            CX_TRAIT(CX_TRAIT_TRIVIALLY_COPYABLE, std::is_trivially_copyable_v<quat>) |
            CX_TRAIT(CX_TRAIT_DEFAULT_CONSTRUCTIBLE, std::is_default_constructible_v<quat>)
        );

}