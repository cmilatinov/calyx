#type vertex
#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec3 pass_color;

void main() {
    pass_color = color;
    gl_Position = vec4(pos, 1);
}

#type fragment
#version 330 core

in vec3 pass_color;

out vec4 color;

void main() {
    color = vec4(pass_color, 1);
}