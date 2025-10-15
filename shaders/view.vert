#version 430 core

layout (location = 0) in vec2 pos;

layout (std140, binding = 2) uniform view_data {
    vec4 heights[1280];
};

uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    float width = 2.0f / 1280;
    float height = heights[gl_InstanceID].x;
    
    float x = (gl_InstanceID + 0.5) * width;
    float y =  -height / 2.0f + pos.y * height;

    gl_Position = projection * view * vec4(x, y, 0.0, 1.0);
    vertex_color = vec3(1.0, 0.0, 0.0);    
}

