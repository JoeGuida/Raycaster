#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <Windows.h>
#include <GL/GL.h>
#include <glcorearb.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "gl_loader.hpp"
#include "point.hpp"
#include "rect.hpp"

#include <cstdint>
#include <span>

struct Renderer {
    uint32_t rect_vao;
    uint32_t rect_vbo;
    uint32_t rect_ebo;

    uint32_t point_vao;
    uint32_t point_vbo;

    uint32_t ssbo[2];

    std::vector<Rect> rect_instances;
    std::vector<float> rect_vertices;
    std::vector<uint32_t> rect_indices;

    std::vector<Point> point_instances;
    std::vector<float> point_vertices;
    std::vector<uint32_t> point_indices;
};

void draw_instanced(Renderer& renderer, GLenum mode, size_t num_indices, size_t num_instances);
void initialize_buffers(Renderer& renderer);
void render_update(Renderer& renderer);
void setup_buffers(Renderer& renderer);
void setup_rects(Renderer& renderer, const std::vector<Rect>& rects);

#endif
