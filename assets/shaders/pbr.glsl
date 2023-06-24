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
    pass_color = vec4(abs(normal), 1);
}

#pragma shader fragment
#version 330 core

in vec3 pass_position;
in vec3 pass_normal;
in vec4 pass_color;

out vec4 color;

//struct PointLight {
//    vec4 color;
//    vec3 position;
//    float radius;
//};
//
//struct DirectionalLight {
//    vec4 color;
//    vec3 direction;
//};
//
//const int MAX_LIGHTS = 3;
//
//uniform PointLight pointLights[MAX_LIGHTS];
//uniform int nPointLights;
//uniform DirectionalLight directionalLights[MAX_LIGHTS];
//uniform int nDirectionalLights;
//
//float Attenuation(PointLight light, float distance) {
//    return pow(max(1.0 - distance / light.radius, 0.0), 2.0);
//}
//
//vec3 IncomingLight(PointLight light) {
//    return Attenuation(light, distance(light.position, pass_position)) * light.color.xyz;
//}
//
//vec3 IncomingLight(DirectionalLight light) {
//    return light.color.xyz;
//}
//
//float LambertFactor(PointLight light) {
//    return max(dot(normalize(light.position - pass_position), normalize(pass_normal)), 0.0);
//}
//
//float LambertFactor(DirectionalLight light) {
//    return max(dot(normalize(-light.direction), normalize(pass_normal)), 0.0);
//}
//
//void Fresnel() {
//
//}
//
//void BRDF() {
//    float ks = 1;
//    float kd = 1 - ks;
//}

void main() {
    color = pass_color;
}