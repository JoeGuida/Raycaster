#version 430 core

layout (location = 0) in vec2 pos;

layout (std140, binding = 0) uniform instance_data {
    vec2 positions[256];
};

uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    vec2 instance_pos = positions[gl_InstanceID].xy;
    gl_Position = projection * view * vec4(pos + instance_pos, 0.0, 1.0);
    vertex_color = vec3(1.0, 0.0, 0.0);
}

