#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <array>
#include <cstdint>

#include <Windows.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "window.hpp"

constexpr int MAX_OBJECTS = 256;
constexpr int RECT_INDICES_COUNT = 6;
constexpr int RECT_VERTICES_COUNT = 4;

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ubo;

    size_t count;

    std::array<glm::vec2, RECT_VERTICES_COUNT> vertices;
    std::array<uint32_t, RECT_INDICES_COUNT> indices;
    std::array<glm::vec4, MAX_OBJECTS> positions;
    std::array<glm::vec4, MAX_OBJECTS> colors;
};

bool initialize_window(Window& window, Renderer& renderer, HINSTANCE instance, int show_window_flags, 
                       int width, int height, const wchar_t* class_name, const wchar_t* window_title);
LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void run_message_loop(Window& window, Renderer& renderer);
void initialize_buffers(Renderer& renderer);
void draw(Renderer& renderer);
void setup(Renderer& renderer);

#endif

