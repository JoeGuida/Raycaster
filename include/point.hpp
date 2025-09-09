#ifndef RAYCASTER_POINT_HPP
#define RAYCASTER_POINT_HPP

#include <glm/vec3.hpp>

struct Point {
    glm::vec3 position;
    glm::vec3 color;
    float size;

    Point(const glm::vec3& position, const glm::vec3& color, float size) : position(position), color(color), size(size) {}
};

#endif
