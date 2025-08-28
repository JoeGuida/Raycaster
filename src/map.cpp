#include "map.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

void load_map_from_file(Map& map, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;

    uint32_t width;
    uint32_t height;

    while(std::getline(file, line)) {
        size_t width_str = line.find("width: ");
        size_t height_str = line.find("height: ");

        if(width_str != std::string::npos) {
            int start = width_str + 7;
            map.width = std::stoi(line.substr(width_str + 7, line.length() - start));
            continue;
        }

        if(height_str != std::string::npos) {
            int start = height_str + 8;
            map.height = std::stoi(line.substr(height_str + 8, line.length() - start));
            continue;
        }
        
        if(!line.empty()) {
            map.data += line; 
        }
    }

    assert(map.width * map.height == map.data.length());

    file.close();
}

