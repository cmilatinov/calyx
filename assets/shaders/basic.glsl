#pragma shader vertex
#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 6) in mat4 transform;

out vec3 pass_color;

uniform mat4 view;

void main() {
    pass_color = abs(normal);
    gl_Position = view * transform * vec4(vertex, 1);
}

#pragma shader fragment
#version 330 core

in vec3 pass_color;

out vec4 color;

void main() {
    color = vec4(pass_color, 1);
}