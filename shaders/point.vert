#version 430 core

layout (location = 0) in vec3 pos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 color;
uniform float size;

out vec3 vertex_color;

void main() {
    gl_Position = projection * view * vec4(pos, 1.0);
    vertex_color = color; 
    gl_PointSize = size;
}
