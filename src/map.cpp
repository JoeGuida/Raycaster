#include "map.hpp"

#include <cassert>
#include <fstream>
#include <string>

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

