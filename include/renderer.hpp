#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <cstdint>
#include <vector>

struct Renderer {
    uint32_t id = 0;

    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ssbo;

    std::vector<float> static_buffer_data;
    std::vector<uint32_t> static_index_data;
};

void render_update(const Renderer& renderer) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uint32_t num_indices = 6;
    uint32_t num_elements = 1;

    for(auto& vertex : renderer.static_buffer_data) {
        draw(renderer.vao, renderer.vbo, renderer.ebo, num_indices, num_elements);
    }
}

void setup_rect(std::span<float> vertices, std::span<float> indices, uint32_t vao, uint32_t vbo, uint32_t ebo); 
void setup_vao(uint32_t vao, uint32_t vbo, uint32_t ssbo); 
void setup(uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t ssbo, std::span<float> vertices, std::span<uint32_t> indices, std::span<glm::vec3> positions); 
void draw(uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t num_indices, uint32_t num_elements); 

#endif
