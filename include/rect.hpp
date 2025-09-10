#ifndef RAYCASTER_RECT_HPP
#define RAYCASTER_RECT_HPP

#include <glm/vec4.hpp>

#include "material.hpp"

#include <cstdint>

struct Rect {
    glm::vec4 position;
    Material material;

    Rect(const glm::vec4& position, const Material& material) : position(position), material(material) {}
};

#endif
