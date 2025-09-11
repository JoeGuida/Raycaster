#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <Windows.h>
#include <GL/GL.h>
#include <glcorearb.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "gl_loader.hpp"
#include "line.hpp"
#include "point.hpp"
#include "rect.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

struct Renderer {
    uint32_t rect_vao;
    uint32_t rect_vbo;
    uint32_t rect_ebo;

    uint32_t point_vao;
    uint32_t point_vbo;
    uint32_t point_ebo;

    uint32_t line_vao;
    uint32_t line_vbo;
    uint32_t line_ebo;

    uint32_t ssbo[2];

    std::vector<Rect> rect_instances;
    std::vector<float> rect_vertices;
    std::vector<uint32_t> rect_indices;

    std::vector<Point> point_instances;
    std::vector<float> point_vertices;
    std::vector<uint32_t> point_indices;

    std::vector<Line> line_instances;
    std::vector<float> line_vertices;
    std::vector<uint32_t> line_indices;

    std::unordered_map<std::string, uint32_t> shaders;
};

void draw_instanced(Renderer& renderer, GLenum mode, size_t num_indices, size_t num_instances);
void initialize_buffers(Renderer& renderer);
void render_update(Renderer& renderer);
void setup_buffers(Renderer& renderer);
void setup_rects(Renderer& renderer, const std::vector<Rect>& rects);
void setup_points(Renderer& renderer, const std::vector<Point>& points);

#endif
