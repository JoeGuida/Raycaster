#ifndef RAYCASTER_RECT_HPP
#define RAYCASTER_RECT_HPP

#include <glm/vec3.hpp>

#include <vector>

struct Rect {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> sizes;
};

#endif
