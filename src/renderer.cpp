#include "renderer.hpp"

#include <Windows.h>
#include <GL/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>

#include "gl_loader.hpp"

#include <cstdint>
#include <span>

void render_update(const Renderer& renderer) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uint32_t num_indices = 6;
    uint32_t num_elements = 1;

    for(auto& vertex : renderer.static_buffer_data) {
        draw(renderer.vao, renderer.vbo, renderer.ebo, num_indices, num_elements);
    }
}

void setup_rect(std::span<float> vertices, std::span<float> indices, uint32_t vao, uint32_t vbo, uint32_t ebo) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);
}

void setup_vao(uint32_t vao, uint32_t vbo, uint32_t ssbo) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void setup(uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t ssbo, std::span<float> vertices, std::span<uint32_t> indices, std::span<glm::vec3> positions) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
}

void draw(uint32_t vao, uint32_t vbo, uint32_t ebo, uint32_t num_indices, uint32_t num_elements) {
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElementsInstanced(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0, num_elements); 
}

