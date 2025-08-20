#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <glm/vec3.hpp>

#include <cstdint>
#include <vector>

struct Renderer {
    std::vector<glm::vec3> static_buffer_data;
    std::vector<uint32_t> static_index_data;

    std::vector<uint32_t> element_data_sizes;
    std::vector<uint32_t> element_index_sizes;
};

void render_update(const Renderer& renderer);

#endif
