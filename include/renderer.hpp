#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <array>
#include <cstdint>

#include <Windows.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "constants.hpp"
#include "window.hpp"

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ubo;
    uint32_t line_ubo;

    uint32_t rect_shader;
    uint32_t line_shader;
    uint32_t point_shader;

    size_t rect_count;
    size_t line_count;
    size_t point_count;

    std::array<glm::vec2, TOTAL_VERTICES_COUNT> vertices;
    std::array<uint32_t, RECT_INDICES_COUNT> indices = { 0, 2, 1, 1, 2, 3 };

    std::array<glm::vec4, MAX_OBJECTS> positions;
    std::array<glm::vec4, MAX_OBJECTS> colors;
    std::array<glm::vec4, MAX_LINES> line_data;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void run_message_loop(Window& window, Renderer& renderer);
void initialize(Renderer& renderer);
void draw(Renderer& renderer);
void setup(Renderer& renderer);

#endif

