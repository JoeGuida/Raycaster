#include "math.hpp"

#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

float cross2d(const glm::vec2& a, const glm::vec2& b) {
    return a.x * b.y - a.y * b.x;
}

std::optional<float> raycast(const glm::vec4& ray, const glm::vec4& line) {
    glm::vec2 a(line.x, line.y);
    glm::vec2 e(line.z - line.x, line.w - line.y);
    glm::vec2 o(ray.x, ray.y);
    glm::vec2 d = glm::vec2(ray.z, ray.w) - glm::vec2(ray.x, ray.y);
    
    float denom = cross2d(d, e);
    if(abs(denom) < FLT_EPSILON) {
        return std::nullopt; 
    }

    float t = cross2d(a - o, e) / denom;
    float u = cross2d(a - o, d) / denom;
    
    if(t >= 0.0f && t <= 1.0f && (u >= 0.0f && u <= 1.0f)) {
        return t;
    }

    return std::nullopt;
}

