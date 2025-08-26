#version 430 core

layout (location = 0) in vec3 pos;
layout(std430, binding = 0) buffer instance_buffer {
    vec4 positions[];
};

uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;

out vec3 vertex_color;

void main() {
    vec3 instance_pos = positions[gl_InstanceID].xyz;
    gl_Position = projection * view * vec4(pos + instance_pos, 1.0);
    vertex_color = color;
}
