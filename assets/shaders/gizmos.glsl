#pragma shader vertex
#version 330 core
#extension GL_ARB_shading_language_include: require

#include "mesh"

out vec3 pass_position;
out vec3 pass_normal;
out vec4 pass_color;

uniform mat4 view;

void main() {
    gl_Position = view * transform * vec4(vertex, 1);
    pass_position = (transform * vec4(vertex, 1)).xyz;
    pass_normal = (transform * vec4(normal, 0)).xyz;
    pass_color = vec4(uv0, uv1);
}

#pragma shader fragment
#version 330 core

in vec3 pass_position;
in vec3 pass_normal;
in vec4 pass_color;

out vec4 fragment;

uniform bool useMeshColors;
uniform bool enableNormals;
uniform vec3 viewPos;
uniform vec4 color;

const float normalFactor = 0.5f;

void main() {
    vec3 toCamera = normalize(viewPos - pass_position);
    vec3 normal = normalize(pass_normal);
    fragment = useMeshColors ? pass_color : color;
    if (enableNormals && dot(normal, toCamera) < 0.0f) {
        fragment.a *= normalFactor;
    }
}
