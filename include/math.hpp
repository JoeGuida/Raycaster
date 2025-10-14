#ifndef RAYCASTER_MATH_HPP
#define RAYCASTER_MATH_HPP

#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>

float cross2d(const glm::vec2& a, const glm::vec2& b);
std::optional<float> raycast(const glm::vec4& ray, const glm::vec4& line);

#endif
