#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "rect.hpp"

#include <cstdint>
#include <span>

struct Renderer {
    uint32_t rect_vao;
    uint32_t rect_vbo;
    uint32_t rect_ebo;

    uint32_t ssbo[2];

    std::vector<Rect> rect_instances;
    std::vector<float> rect_vertices;
    std::vector<uint32_t> rect_indices;
};

void draw_instanced(Renderer& renderer); 
void initialize_buffers(Renderer& renderer); 
void render_update(Renderer& renderer); 
void setup_buffers(Renderer& renderer); 
void setup_rects(Renderer& renderer, const std::vector<Rect> rects); 

#endif
