
#include "assets/Assembly.h"

CX_REFLECT_REGISTRATION {
    using namespace entt::literals;

    entt::meta<vec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec2).data()))
        .data<&vec2::x>("x"_hs)
        .data<&vec2::y>("y"_hs);

    entt::meta<vec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec3).data()))
        .data<&vec3::x>("x"_hs)
        .data<&vec3::y>("y"_hs)
        .data<&vec3::z>("z"_hs);

    entt::meta<vec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(vec4).data()))
        .data<&vec4::x>("x"_hs)
        .data<&vec4::y>("y"_hs)
        .data<&vec4::z>("z"_hs)
        .data<&vec4::w>("w"_hs);

    entt::meta<ivec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec2).data()))
        .data<&ivec2::x>("x"_hs)
        .data<&ivec2::y>("y"_hs);

    entt::meta<ivec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec3).data()))
        .data<&ivec3::x>("x"_hs)
        .data<&ivec3::y>("y"_hs)
        .data<&ivec3::z>("z"_hs);

    entt::meta<ivec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(ivec4).data()))
        .data<&ivec4::x>("x"_hs)
        .data<&ivec4::y>("y"_hs)
        .data<&ivec4::z>("z"_hs)
        .data<&ivec4::w>("w"_hs);

    entt::meta<uvec2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec2).data()))
        .data<&uvec2::x>("x"_hs)
        .data<&uvec2::y>("y"_hs);

    entt::meta<uvec3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec3).data()))
        .data<&uvec3::x>("x"_hs)
        .data<&uvec3::y>("y"_hs)
        .data<&uvec3::z>("z"_hs);

    entt::meta<uvec4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(uvec4).data()))
        .data<&uvec4::x>("x"_hs)
        .data<&uvec4::y>("y"_hs)
        .data<&uvec4::z>("z"_hs)
        .data<&uvec4::w>("w"_hs);

    entt::meta<mat2>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat2).data()));

    entt::meta<mat3>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat3).data()));

    entt::meta<mat4>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(mat4).data()));

    entt::meta<quat>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(quat).data()));

    entt::meta<String>()
        .type(entt::hashed_string(NAMEOF_SHORT_TYPE(String).data()));

}