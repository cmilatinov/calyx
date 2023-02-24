#type vertex
#version 330 core

layout (location = 0) in vec3 vertex;

out vec3 nearPoint;
out vec3 farPoint;

uniform mat4 invView;

vec3 ToWorldSpace(vec3 clipCoords) {
    vec4 point = invView * vec4(clipCoords, 1.0);
    return point.xyz / point.w;
}

void main() {
    vec2 point = vertex.xy;
    nearPoint = ToWorldSpace(vec3(point, 0.0));
    farPoint = ToWorldSpace(vec3(point, 1.0));
    gl_Position = vec4(point, 0.0, 1.0);
}

#type fragment
#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

out vec4 color;

uniform mat4 view;
uniform float nearPlane;
uniform float farPlane;

vec4 Grid(vec3 fragPos, vec3 gridColor, float lineWidth, float scale) {
    vec2 coord = fragPos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = max(abs(fract(coord - 0.5) - 0.5) - lineWidth, 0) / derivative;
    float line = min(grid.x, grid.y);
    vec4 color = vec4(gridColor, 1.0 - min(line, 1.0));

    float minX = min(derivative.x, 1);
    float minZ = min(derivative.y, 1);
    if (fragPos.x > -minX && fragPos.x < minX) {
        color.xyz = vec3(0.1, 0.1, 1.0);
    } else if (fragPos.z > -minZ && fragPos.z < minZ) {
        color.xyz = vec3(1.0, 0.1, 0.1);
    }

    return color;
}

float ComputeDepth(vec3 fragPos) {
    vec4 clipSpace = view * vec4(fragPos, 1.0);
    return ((1 / clipSpace.z) - (1 / nearPlane)) / ((1 / farPlane) - (1 / nearPlane));
}

float LinearizeDepth(float depth) {
    float z = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane)) / farPlane;
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t <= 0)
        discard;

    vec3 fragPos = nearPoint + t * (farPoint - nearPoint);
    color = Grid(fragPos, vec3(0.5), 0, 1) + Grid(fragPos, vec3(0.1), 0.0001, 0.1);

    float depth = ComputeDepth(fragPos);
    float linearDepth = LinearizeDepth(depth);
    float alpha = 1 - (5 * linearDepth * linearDepth);
    color.a *= alpha;

    gl_FragDepth = color.a > 0 ? depth : 0.999;
}
