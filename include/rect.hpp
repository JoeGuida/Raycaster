#ifndef RAYCASTER_RECT_HPP
#define RAYCASTER_RECT_HPP

#include <glm/vec4.hpp>

#include <cstdint>

struct Rect {
    glm::vec4 position;
    uint32_t color;

    Rect(const glm::vec4& position, const uint32_t& color) : position(position), color(color) {}
};

#endif
