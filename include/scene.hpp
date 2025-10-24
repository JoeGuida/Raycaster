#ifndef RAYCASTER_SCENE_HPP
#define RAYCASTER_SCENE_HPP

#include <cstdint>
#include <string>
#include <unordered_map>

#include <glm/vec3.hpp>

#include "camera.hpp"
#include "map.hpp"

struct Scene {
    uint32_t width;
    uint32_t height;
    glm::vec3 background_color;
    Camera camera; 
    Map map;
    std::unordered_map<std::string, uint32_t> shaders;
};

Scene load_scene_from_file(const std::string& filepath);

#endif
