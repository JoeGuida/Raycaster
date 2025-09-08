#include "renderer.hpp"

#include <Windows.h>
#include <GL/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "gl_loader.hpp"
#include "rect.hpp"

#include <cstdint>
#include <ranges>
#include <vector>

constexpr float RECT_SIZE = 2.0f / 16.0f;
constexpr float HALF_SIZE = RECT_SIZE / 2.0f;

void draw_instanced(Renderer& renderer) {
    glDrawElementsInstanced(GL_TRIANGLES, renderer.rect_indices.size(), GL_UNSIGNED_INT, 0, renderer.rect_instances.size()); 
}

void initialize_buffers(Renderer& renderer) {
    glGenVertexArrays(1, &renderer.rect_vao);
    glGenBuffers(1, &renderer.rect_vbo);
    glGenBuffers(1, &renderer.rect_ebo);
    glGenBuffers(1, &renderer.ssbo[0]);
    glGenBuffers(1, &renderer.ssbo[1]);
}

void render_update(Renderer& renderer) {
    glClearColor(0.216f, 0.212f, 0.310f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(renderer.rect_vao);
    draw_instanced(renderer);
}

void setup_buffers(Renderer& renderer) {
    glBindVertexArray(renderer.rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.rect_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void setup_rects(Renderer& renderer, const std::vector<Rect> rects) {
    renderer.rect_instances = rects;
    renderer.rect_indices = { 0, 1, 2, 1, 3, 2 };
    renderer.rect_vertices = std::vector<float> { 
        -HALF_SIZE,  HALF_SIZE, 0.0f,
         HALF_SIZE,  HALF_SIZE, 0.0f,
        -HALF_SIZE, -HALF_SIZE, 0.0f,
         HALF_SIZE, -HALF_SIZE, 0.0f
    };

    std::vector<glm::vec4> positions = rects
                                     | std::views::transform([](const Rect& r) { return r.position; })
                                     | std::ranges::to<std::vector<glm::vec4>>();

    std::vector<uint32_t> colors = rects
                                 | std::views::transform([](const Rect& r) { return r.color; })
                                 | std::ranges::to<std::vector<uint32_t>>();
 
    glBindVertexArray(renderer.rect_vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.rect_vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer.rect_vertices.size() * sizeof(float), renderer.rect_vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.rect_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderer.rect_indices.size() * sizeof(uint32_t), renderer.rect_indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer.ssbo[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, renderer.ssbo[0]);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, renderer.ssbo[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, colors.size() * sizeof(uint32_t), colors.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, renderer.ssbo[1]);
}

