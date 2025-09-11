#ifndef RAYCASTER_LINE_HPP
#define RAYCASTER_LINE_HPP

#include <glm/vec4.hpp>

#include "material.hpp"

struct Line {
    glm::vec4 position; 
    Material material;

    Line(const glm::vec4& position, const Material& material) : position(position), material(material) {}
};

#endif
