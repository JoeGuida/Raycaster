#ifndef RAYCASTER_MAP_HPP
#define RAYCASTER_MAP_HPP

#include <array>
#include <print>
#include <string>

#include <glm/vec4.hpp>

#include "renderer.hpp"

struct Map {
    uint32_t width;
    uint32_t height;
    std::string data;

    Map() : width(0), height(0), data("") {}
};

void load_map_from_file(Map& map, const std::string& filepath);
void write_map_to_buffers(const Map& map, Renderer& renderer, float aspect, const std::array<glm::vec4, 10>& color_palette);

#endif

