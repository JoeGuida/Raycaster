#ifndef RAYCASTER_CAMERA_HPP
#define RAYCASTER_CAMERA_HPP

#include <glm/vec2.hpp>

#include "renderer.hpp"

struct Camera {
    glm::vec2 position;
    glm::vec2 direction;
    float fov;
};

void write_camera_fov_to_buffers(Renderer& renderer, const Camera& camera);
void raycast_fov(Renderer& renderer, const glm::vec2& direction);

#endif
