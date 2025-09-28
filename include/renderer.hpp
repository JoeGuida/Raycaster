#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <cstdint>
#include <vector>

#include <Windows.h>

#include <glm/vec3.hpp>

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ubo;

    std::vector<glm::vec3> vertices;
    std::vector<uint32_t> indices;
    std::vector<glm::vec3> positions;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
void initialize_buffers(Renderer& renderer);
void draw(Renderer& renderer);

#endif

