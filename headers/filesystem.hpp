#ifndef RAYCASTER_FILESYSTEM_HPP
#define RAYCASTER_FILESYSTEM_HPP

#include <cstdlib>
#include <iostream>
#include <string>

class FileSystem final {
private:
    static std::string get_root() {
        const char* env = std::getenv("RAYCASTER_ROOT");

        if (!env) {
            std::cout << "ERROR::FILESYSTEM Error reading environment variable: RAYCASTER_ROOT" << std::endl;
            return nullptr;
        }
        return std::string(env);
    }

public:
    static std::string get_path(const char* path) {
        std::string root = get_root();

        if (root.empty())
            return nullptr;

        return std::string(root + path);
    }
};

#endif