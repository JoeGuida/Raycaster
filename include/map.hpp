#ifndef RAYCASTER_MAP_HPP
#define RAYCASTER_MAP_HPP

#include <format>
#include <iostream>
#include <string>

struct Map {
    uint32_t width;
    uint32_t height;
    std::string data;

    Map() : width(0), height(0), data("") {}

    char operator[](uint32_t i) {
        if(i >= data.length()) {
            std::cerr << std::format("Error accessing map data at index {}, Map Size: {}", i, data.length()) << std::endl;
        }

        return data[i];
    }
};

void load_map_from_file(Map& map, const std::string& filepath);

#endif
