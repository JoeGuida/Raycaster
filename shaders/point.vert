#version 430 core

layout (location = 0) in vec2 pos;

layout (std140, binding = 0) uniform instance_data {
    vec4 positions[256 * 3];
    vec4 colors[256 * 3];
};

uniform mat4 view;
uniform mat4 projection;
uniform float size;

out vec3 vertex_color;

const uint POINT_OFFSET = 512;

void main() {
    vec2 instance_pos = positions[gl_InstanceID + POINT_OFFSET].xy;
    gl_Position = projection * view * vec4(pos + instance_pos, 0.0, 1.0);
    vertex_color = colors[gl_InstanceID + POINT_OFFSET].xyz;
    gl_PointSize = size;
}

