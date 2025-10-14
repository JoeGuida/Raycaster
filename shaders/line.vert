#version 430 core

layout (location = 0) in vec2 pos;

layout (std140, binding = 1) uniform line_data {
    vec4 positions[2048];
};

uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    vec2 start = positions[gl_InstanceID].xy;
    vec2 end = positions[gl_InstanceID].zw;
    vec2 vertex = gl_VertexID % 2 == 0 ? start : end;
    gl_Position = projection * view * vec4(vertex, 0.0, 1.0);
    vertex_color = vec3(1.0, 1.0, 1.0);    
}

