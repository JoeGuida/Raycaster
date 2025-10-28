#ifndef RAYCASTER_SCENE_HPP
#define RAYCASTER_SCENE_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/vec3.hpp>
#include <yaml-cpp/yaml.h>

#include "camera.hpp"
#include "instance_data.hpp"
#include "map.hpp"

struct Scene {
    uint32_t width;
    uint32_t height;
    glm::vec3 background_color;
    Camera camera; 
    Map map;
    std::unordered_map<std::string, uint32_t> shaders;
    std::vector<glm::vec3> palette;
    std::vector<InstanceData> points;
    std::vector<InstanceData> lines;
    std::vector<InstanceData> rects;
};

std::optional<uint32_t> parse_uint32_t(const YAML::Node& node);
std::optional<float> parse_float(const YAML::Node& node);
std::optional<std::string> parse_string(const YAML::Node& node);
std::optional<glm::vec2> parse_vec2(const YAML::Node& node);
std::optional<glm::vec3> parse_vec3(const YAML::Node& node);
std::optional<Camera> parse_camera(const YAML::Node& node, int width, int height);
std::optional<Map> parse_map(const YAML::Node& node);
std::optional<std::unordered_map<std::string, uint32_t>> parse_shaders(const YAML::Node& node);
std::optional<std::vector<glm::vec3>> parse_palette(const YAML::Node& node);
void parse_objects(const YAML::Node& node, std::vector<InstanceData>& points, std::vector<InstanceData>& lines, std::vector<InstanceData>& rects);
Scene load_scene_from_file(const std::string& filepath);

#endif
