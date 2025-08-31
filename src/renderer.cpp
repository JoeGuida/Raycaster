#include "renderer.hpp"

#include <Windows.h>
#include <GL/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "gl_loader.hpp"

#include <cstdint>
#include <span>

void render_update(Renderer& renderer) {
    glClearColor(0.216f, 0.212f, 0.310f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(renderer.vao);
    draw(renderer);
}

void initialize_buffers(Renderer& renderer) {
    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ebo);
    glGenBuffers(1, &renderer.ssbo[0]);
    glGenBuffers(1, &renderer.ssbo[1]);
}

void setup_buffers(Renderer& renderer) {
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void setup_instanced_elements(Renderer& renderer, std::span<float> vertices, std::span<uint32_t> indices, std::span<glm::vec4> positions, std::span<uint32_t> colors) {
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer.ssbo[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderer.ssbo[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer.ssbo[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, colors.size() * sizeof(uint32_t), colors.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, renderer.ssbo[1]);

    renderer.num_indices = indices.size();
    renderer.num_elements = positions.size();
}

void draw(Renderer& renderer) {
    glDrawElementsInstanced(GL_TRIANGLES, renderer.num_indices, GL_UNSIGNED_INT, 0, renderer.num_elements); 
}

