#include "rect.hpp"

#include <glm/vec3.hpp>

#include <span>

void calculate_vertices(std::span<float> vertices, std::span<glm::vec3> sizes) {
    for(const auto& size : sizes) {
        const glm::vec3 half = size / 2.0f;
        std::array<float, 12> v = {
            -half.x,  half.y, 0.0f,
             half.x,  half.y, 0.0f,
            -half.x, -half.y, 0.0f,
             half.x, -half.y, 0.0f
        };
           
    }
}
