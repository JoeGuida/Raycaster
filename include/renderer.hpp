#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <array>
#include <cstdint>
#include <span>
#include <vector>

struct Renderer {
    uint32_t id = 0;

    uint32_t rect_vao;
    uint32_t rect_vbo;
    uint32_t rect_ebo;

    uint32_t point_vao;
    uint32_t point_vbo;

    std::array<uint32_t, 2> ssbo;

    uint32_t num_indices;
    uint32_t num_elements;
};

void render_update(Renderer& renderer); 
void initialize_buffers(Renderer& renderer); 
void setup_buffers(Renderer& renderer); 
void setup_point(Renderer& renderer, const glm::vec3& position, const glm::vec3& color, float size); 
void setup_instanced_elements(Renderer& renderer, std::span<float> vertices, std::span<uint32_t> indices, std::span<glm::vec4> positions, std::span<uint32_t> colors); 
void draw_instanced(Renderer& renderer); 
void draw(Renderer& renderer); 

#endif
