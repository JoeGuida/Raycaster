#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <array>
#include <cstdint>
#include <vector>

#include <Windows.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "constants.hpp"
#include "window.hpp"

constexpr size_t POSITIONS_SIZE = sizeof(glm::vec4) * MAX_OBJECTS;
constexpr size_t COLORS_SIZE    = sizeof(glm::vec4) * MAX_OBJECTS;

constexpr size_t RECT_UBO_SIZE  = POSITIONS_SIZE + COLORS_SIZE;
constexpr size_t LINE_UBO_SIZE  = sizeof(glm::vec4) * MAX_LINES;
constexpr size_t VIEW_UBO_SIZE  = sizeof(glm::vec4) * SCREEN_WIDTH;

constexpr size_t LINE_INDEX = 4;
constexpr size_t POINT_INDEX = 6;

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t rect_ubo;
    uint32_t line_ubo;
    uint32_t view_ubo;

    size_t rect_count;
    size_t line_count;
    size_t point_count;
    size_t view_count;

    std::array<glm::vec2, TOTAL_VERTICES_COUNT> vertices = {
        glm::vec2(-1.0f,  1.0f),
        glm::vec2( 1.0f,  1.0f),
        glm::vec2(-1.0f, -1.0f),
        glm::vec2( 1.0f, -1.0f),
        glm::vec2( 0.0f,  0.0f),
        glm::vec2( 0.0f,  1.0f),
        glm::vec2( 0.0f,  0.0f)
    };

    std::array<uint32_t, RECT_INDICES_COUNT> indices = {
        0, 2, 1,
        1, 2, 3
    };

    std::array<glm::vec4, MAX_OBJECTS> positions;
    std::array<glm::vec4, MAX_OBJECTS> colors;
    std::array<glm::vec4, MAX_LINES> line_data;
    std::array<glm::vec4, SCREEN_WIDTH> view_data;

    std::unordered_map<std::string, uint32_t> shaders;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void run_message_loop(Window& window, Renderer& renderer, const glm::vec3& background_color);
void enable(const Renderer& renderer, const std::vector<uint32_t>& features);
void initialize(Renderer& renderer);
void draw(Renderer& renderer);
void setup(Renderer& renderer);

#endif

