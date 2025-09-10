#ifndef RAYCASTER_MATERIAL_HPP
#define RAYCASTER_MATERIAL_HPP

#include <glm/vec4.hpp>

struct Material {
    glm::vec4 color;

    Material(const glm::vec4& color) : color(color) {}
};

#endif
