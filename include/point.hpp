#ifndef RAYCASTER_POINT_HPP
#define RAYCASTER_POINT_HPP

#include <glm/vec3.hpp>

#include "material.hpp"

struct Point {
    glm::vec3 position;
    float size;
    Material material;

    Point(const glm::vec3& position, float size, const Material& material) : position(position), size(size), material(material) {}
};

#endif
