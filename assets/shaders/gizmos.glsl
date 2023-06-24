#pragma shader vertex
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 6) in mat4 transform;

out vec3 pass_position;
out vec3 pass_normal;

uniform mat4 view;

void main() {
    gl_Position = view * transform * vec4(vertex, 1);
    pass_position = (transform * vec4(vertex, 1)).xyz;
    pass_normal = (transform * vec4(normal, 0)).xyz;
}

#pragma shader fragment
#version 330 core

in vec3 pass_position;
in vec3 pass_normal;

out vec4 fragment;

uniform bool enableNormals;
uniform vec3 viewPos;
uniform vec4 color;

const float normalFactor = 0.5f;

void main() {
    vec3 toCamera = normalize(viewPos - pass_position);
    vec3 normal = normalize(pass_normal);
    fragment = color;
    if (enableNormals && dot(normal, toCamera) < 0.0f) {
        fragment.a *= normalFactor;
    }
}
