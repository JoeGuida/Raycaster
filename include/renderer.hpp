#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <glm/vec4.hpp>

#include <array>
#include <cstdint>
#include <span>
#include <vector>

struct Renderer {
    uint32_t id = 0;

    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    std::array<uint32_t, 2> ssbo;

    std::vector<float> static_buffer_data;
    std::vector<uint32_t> static_index_data;

    uint32_t num_indices;
    uint32_t num_elements;
};

void render_update(Renderer& renderer); 
void initialize_buffers(Renderer& renderer); 
void setup_buffers(Renderer& renderer); 
void setup_instanced_elements(Renderer& renderer, std::span<float> vertices, std::span<uint32_t> indices, std::span<glm::vec4> positions, std::span<uint32_t> colors); 
void draw(Renderer& renderer); 

#endif
