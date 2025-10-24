#include "scene.hpp"

#include <filesystem>
#include <optional>
#include <string>

#include <Windows.h>
#include <opengl/glcorearb.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "map.hpp"
#include "shader.hpp"

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

std::optional<std::string> parse_string(const YAML::Node& node) {
    if(node && node.IsScalar()) {
        return node.as<std::string>();
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
    if(cam_pos.has_value() && cam_dir.has_value() && cam_fov.has_value()) {
        camera.position = cam_pos.value();
        camera.direction = cam_dir.value();
        camera.fov = cam_fov.value();
        camera.aspect = width.value() / static_cast<float>(height.value());
    }

    scene.camera = camera;

    Map map;
    auto map_file = parse_string(yaml_scene["map"]);
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path map_path = current_path / "maps";
    if(map_file.has_value()) {
        std::string s = (map_path / map_file.value()).string();
        load_map_from_file(map, s);
    }

    scene.map = std::move(map);

    std::filesystem::path shader_path = current_path / "shaders"; 
    auto shaders = yaml_scene["shaders"];
    if(shaders.IsSequence()) {
        for(int i = 0; i < shaders.size(); i++) {
            auto shader_name = parse_string(shaders[i]["name"]);
            if(shader_name.has_value()) {
                auto vertex = parse_string(shaders[i]["vertex"]);
                auto fragment = parse_string(shaders[i]["fragment"]);
                
                std::optional<uint32_t> vertex_shader;
                std::optional<uint32_t> fragment_shader;

                if(vertex.has_value()) {
                    auto vertex_id = compile_shader(shader_path.string(), shader_name.value(), GL_VERTEX_SHADER);
                    if(!vertex_id.has_value()) {
                        spdlog::error("{}", vertex_id.error());
                    }
                    else {
                        vertex_shader = vertex_id.value();
                    }
                }
                if(fragment.has_value()) {
                    auto fragment_id = compile_shader(shader_path.string(), shader_name.value(), GL_FRAGMENT_SHADER);
                    if(!fragment_id.has_value()) {
                        spdlog::error("{}", fragment_id.error());
                    }
                    else {
                        fragment_shader = fragment_id.value();
                    }
                }

                if(vertex_shader.has_value() && fragment_shader.has_value()) {
                    auto shader = link_shaders(vertex_shader.value(), fragment_shader.value());
                    if(!shader.has_value()) {
                        spdlog::error("{}", shader.error());
                    }
                    else {
                        scene.shaders[shader_name.value()] = shader.value();
                    }
                }
            }
        }
    }

    return scene;
}

