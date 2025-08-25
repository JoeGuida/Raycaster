#ifndef RAYCASTER_RENDERER_HPP
#define RAYCASTER_RENDERER_HPP

#include <cstdint>
#include <vector>

struct Renderer {
    uint32_t id = 0;
    std::vector<float> static_buffer_data;
    std::vector<uint32_t> static_index_data;
};

void render_update(const Renderer& renderer);

#endif
