#include "map.hpp"

#include <array>
#include <cassert>
#include <fstream>
#include <string>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "renderer.hpp"

void load_map_from_file(Map& map, const std::string& filepath) {
    std::fstream file(filepath);
    std::string line;

    while(std::getline(file, line)) {
        if(map.width == 0) {
            map.width = std::stoi(line);
            continue;
        }

        if(map.height == 0) {
            map.height = std::stoi(line);
            continue;
        }

        if(!line.empty()) {
            map.data += line; 
        }
    }

    assert(map.width * map.height == map.data.length());

    file.close();
}

void write_map_to_buffers(const Map& map, Renderer& renderer, float aspect, const std::array<glm::vec4, 10>& color_palette) {
    glm::vec2 rect_size(aspect / map.width, 2.0f / map.height);
    glm::vec2 half_rect_size = rect_size / 2.0f;
    for(int i = 0; i < map.height; i++) {
        for(int j = 0; j < map.width; j++) {

            int map_index = i * map.width + j;
            if(map.data[map_index] != ' ') {
                glm::vec2 position(
                    (-aspect + half_rect_size.x) + rect_size.x * j,
                    (1.0f - half_rect_size.y) - rect_size.y * i
                );

                renderer.positions[renderer.rect_count] = glm::vec4(position.x, position.y, 0.0f, 0.0f);
                int color_index = map.data[map_index] - '0';

                renderer.colors[renderer.rect_count] = color_palette[color_index];
                renderer.rect_count++;
            }
        }
    }
}
