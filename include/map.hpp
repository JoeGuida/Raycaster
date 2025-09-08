#ifndef RAYCASTER_MAP_HPP
#define RAYCASTER_MAP_HPP

#include <print>
#include <string>

struct Map {
    uint32_t width;
    uint32_t height;
    std::string data;

    Map() : width(0), height(0), data("") {}

    char operator[](uint32_t i) {
        if(i >= data.length()) {
            std::println("Error accessing map data at index {}, Map Size: {}", i, data.length());
        }

        return data[i];
    }

    char operator[](uint32_t i) const {
        if(i >= data.length()) {
            std::println("Error accessing map data at index {}, Map Size: {}", i, data.length());
        }

        return data[i];
    }
};

void load_map_from_file(Map& map, const std::string& filepath);

#endif
