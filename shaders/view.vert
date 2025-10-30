#version 430 core

const int SIZE = 1280;

layout (location = 0) in vec2 pos;

layout (std140, binding = 2) uniform view_data {
    vec4 data[SIZE];
};

uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    float width = 1.7777 / SIZE;
    float height = 2.0 / SIZE;
    float distance = data[gl_InstanceID].x;

    float x = (gl_InstanceID + 0.5) * width;
    float y =  -distance / 2.0 + pos.y * distance;

    gl_Position = projection * view * vec4(x, y, 0.0, 1.0);
    vertex_color = data[gl_InstanceID].yzw; 
}

