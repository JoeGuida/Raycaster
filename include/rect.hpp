#ifndef RAYCASTER_RECT_HPP
#define RAYCASTER_RECT_HPP

#include <glm/vec3.hpp>

#include <array>
#include <cstdint>
#include <span>
#include <vector>

struct Rect {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> sizes;
    std::vector<float> vertices;
    std::array<uint32_t, 6> indices = { 0, 1, 2, 1, 3, 2 };
};

void calculate_vertices(std::span<float> vertices, std::span<glm::vec3> sizes); 

#endif
