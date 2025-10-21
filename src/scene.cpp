#include "scene.hpp"

#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

std::optional<uint32_t> parse_uint32_t(const YAML::Node& node) {
    if(node && node.IsScalar()) {
        return node.as<uint32_t>();
    }

    return std::nullopt;
}

std::optional<float> parse_float(const YAML::Node& node) {
    if(node && node.IsScalar()) {
        return node.as<float>();
    }

    return std::nullopt;
}

std::optional<glm::vec2> parse_vec2(const YAML::Node& node) {
    if(node && node.IsSequence()) {
        if(node.size() == 2 && node[0].IsScalar() && node[1].IsScalar()) {
            return glm::vec2(node[0].as<float>(), node[1].as<float>());
        }
    }

    return std::nullopt;
}

std::optional<glm::vec3> parse_vec3(const YAML::Node& node) {
    if(node && node.IsSequence()) {
        if(node.size() == 3 && node[0].IsScalar() && node[1].IsScalar() && node[2].IsScalar()) {
            return glm::vec3(node[0].as<float>(), node[1].as<float>(), node[2].as<float>());
        }
    }

    return std::nullopt;
}

Scene load_scene_from_file(const std::string& filepath) {
    Scene scene{};
    YAML::Node yaml_scene = YAML::LoadFile(filepath);
    
    auto width = parse_uint32_t(yaml_scene["width"]);
    auto height = parse_uint32_t(yaml_scene["height"]);
    if(width.has_value() && height.has_value()) {
        scene.width = width.value();
        scene.height = height.value();
    }

    auto background_color = parse_vec3(yaml_scene["background_color"]);
    if(background_color.has_value()) {
        scene.background_color = background_color.value();
    }

    Camera camera{};
    auto cam_pos = parse_vec2(yaml_scene["camera"]["position"]);
    auto cam_dir = parse_vec2(yaml_scene["camera"]["direction"]);
    auto cam_fov = parse_float(yaml_scene["camera"]["fov"]);
    if(cam_pos.has_value() && cam_dir.has_value() && cam_fov.has_value() && width.has_value() && height.has_value()) {
        camera.position = cam_pos.value();
        camera.direction = cam_dir.value();
        camera.fov = cam_fov.value();
        camera.aspect = width.value() / static_cast<float>(height.value());
    }

    scene.camera = std::move(camera);

    return scene;
}

