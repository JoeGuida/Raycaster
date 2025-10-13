#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <array>
#include <cstdint>

#include <Windows.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "window.hpp"

constexpr size_t NUM_PRIMITIVES = 3;
constexpr size_t OBJECT_COUNT_PER_PRIMITIVE = 256;
constexpr size_t MAX_OBJECTS = 256 * NUM_PRIMITIVES;

constexpr size_t RECT_INDICES_COUNT  = 6;
constexpr size_t RECT_VERTICES_COUNT = 4;

constexpr size_t LINE_INDICES_COUNT  = 2;
constexpr size_t LINE_VERTICES_COUNT = 2;

constexpr size_t POINT_VERTICES_COUNT = 1;

constexpr size_t TOTAL_VERTICES_COUNT = RECT_VERTICES_COUNT + LINE_VERTICES_COUNT + POINT_VERTICES_COUNT;
constexpr size_t TOTAL_INDICES_COUNT  = RECT_INDICES_COUNT + LINE_INDICES_COUNT;

constexpr uint32_t LINE_OFFSET = OBJECT_COUNT_PER_PRIMITIVE;
constexpr uint32_t POINT_OFFSET = OBJECT_COUNT_PER_PRIMITIVE * 2;

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ubo;

    uint32_t rect_shader;
    uint32_t line_shader;
    uint32_t point_shader;

    size_t rect_count;
    size_t line_count;
    size_t point_count;

    std::array<glm::vec2, TOTAL_VERTICES_COUNT> vertices;
    std::array<uint32_t, TOTAL_INDICES_COUNT> indices;

    std::array<glm::vec4, MAX_OBJECTS> positions;
    std::array<glm::vec4, MAX_OBJECTS> colors;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void run_message_loop(Window& window, Renderer& renderer);
void initialize(Renderer& renderer);
void draw(Renderer& renderer);
void setup(Renderer& renderer);

#endif

