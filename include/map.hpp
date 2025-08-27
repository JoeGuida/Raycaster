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
    Map(uint32_t width, uint32_t height, const std::string& data) : width(width), height(height), data(data) {
        if(width * height != data.length()) {
            std::cerr << std::format("Error Loading Map :: Width = {}, Height = {}, Data Length = {}", width, height, data.length()) << std::endl; 
        }
    }

    char operator[](uint32_t i) {
        if(i >= data.length()) {
            std::cerr << std::format("Error accessing map data at index {}, Map Size: {}", i, data.length()) << std::endl;
        }

        return data[i];
    }
};

void load_map_from_file(Map& map, const std::string& filepath);

#endif
