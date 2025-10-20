#include "convert.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

glm::vec4 to_vec4(const glm::vec2& v) {
    return { v.x, v.y, 0.0f, 0.0f };
}

glm::vec4 to_vec4(const glm::vec3& v) {
    return { v.x, v.y, v.z, 0.0f };
}

