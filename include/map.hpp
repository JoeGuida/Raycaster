#ifndef RAYCASTER_MAP_HPP
#define RAYCASTER_MAP_HPP

#include <print>
#include <string>

struct Map {
    uint32_t width;
    uint32_t height;
    std::string data;

    Map() : width(0), height(0), data("") {}
};

void load_map_from_file(Map& map, const std::string& filepath);

#endif

