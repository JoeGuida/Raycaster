#include "map.hpp"

#include <fstream>
#include <iostream>
#include <string>

void load_map_from_file(Map& map, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;

    while(std::getline(file, line)) {
        std::cout << line << std::endl; 
    }
}

