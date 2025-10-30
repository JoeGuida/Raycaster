#include "scene.hpp"

#include <filesystem>
#include <optional>
#include <string>

#include <Windows.h>
#include <opengl/glcorearb.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>
#include <yaml-cpp/yaml.h>

#include "instance_data.hpp"
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

std::optional<Camera> parse_camera(const YAML::Node& node, int width, int height) {
    Camera camera{};
    auto cam_pos = parse_vec2(node["camera"]["position"]);
    auto cam_dir = parse_vec2(node["camera"]["direction"]);
    auto cam_fov = parse_float(node["camera"]["fov"]);
    if(cam_pos.has_value() && cam_dir.has_value() && cam_fov.has_value()) {
        camera.position = cam_pos.value();
        camera.direction = cam_dir.value();
        camera.fov = glm::radians(cam_fov.value());
        camera.aspect = width / static_cast<float>(height);
    }
    else {
        return std::nullopt;
    }

    return camera;
}

std::optional<Map> parse_map(const YAML::Node& node) {
    Map map;
    auto map_file = parse_string(node["map"]);
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path map_path = current_path / "maps";
    if(!map_file.has_value()) {
        return std::nullopt;
    }

    std::string s = (map_path / map_file.value()).string();
    load_map_from_file(map, s);
    return map;
}

std::optional<std::unordered_map<std::string, uint32_t>> parse_shaders(const YAML::Node& node) {
    std::filesystem::path current_path = std::filesystem::current_path();
    std::filesystem::path shader_path = current_path / "shaders"; 
    auto shaders = node["shaders"];
    std::unordered_map<std::string, uint32_t> shader_map;
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
                        spdlog::error("ERROR: {}", vertex_id.error());
                    }
                    else {
                        vertex_shader = vertex_id.value();
                    }
                }

                if(fragment.has_value()) {
                    auto fragment_id = compile_shader(shader_path.string(), shader_name.value(), GL_FRAGMENT_SHADER);
                    if(!fragment_id.has_value()) {
                        spdlog::error("ERROR: {}", fragment_id.error());
                    }
                    else {
                        fragment_shader = fragment_id.value();
                    }
                }

                if(vertex_shader.has_value() && fragment_shader.has_value()) {
                    auto shader = link_shaders(vertex_shader.value(), fragment_shader.value());
                    if(!shader.has_value()) {
                        spdlog::error("ERROR: {}", shader.error());
                    }
                    else {
                        shader_map[shader_name.value()] = shader.value();
                    }
                }
            }
        }
    }

    return shader_map;
}

std::optional<std::vector<glm::vec3>> parse_palette(const YAML::Node& node) {
    auto palette = node["palette"];
    std::vector<glm::vec3> color_palette;
    if(palette.IsSequence()) {
        color_palette.reserve(palette.size());
        for(int i = 0; i < palette.size(); i++) {
            auto color = parse_vec3(palette[i]);
            if(color.has_value()) {
                color_palette.push_back(color.value());
            }
        }
    }

    return color_palette;
}

void parse_objects(const YAML::Node& node, std::vector<InstanceData>& points, std::vector<InstanceData>& lines, std::vector<InstanceData>& rects) {
    auto objects = node["objects"];
    if(objects.IsSequence()) {
        for(int i = 0; i < objects.size(); i++) {
            InstanceData data;
            auto type = parse_string(objects[i]["type"]);
            auto position = parse_vec2(objects[i]["position"]);
            auto color = parse_vec3(objects[i]["color"]);
           
            if(position.has_value()) {
                data.position = position.value();
            }

            if(color.has_value()) {
                data.color = color.value();
            }

            if(type.has_value()) {
                if(type == "point") {
                    points.push_back(data);
                }
                if(type == "line") {
                    lines.push_back(data);
                }
                if(type == "rect") {
                    rects.push_back(data);
                }
            }
        }
    }
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

    auto camera = parse_camera(yaml_scene, width.value(), height.value());
    if(camera.has_value()) {
        scene.camera = camera.value();
    }

    auto map = parse_map(yaml_scene);
    if(map.has_value()) {
        scene.map = std::move(map).value();
    }

    auto shaders = parse_shaders(yaml_scene);
    if(shaders.has_value()) {
        scene.shaders = std::move(shaders).value();
    }

    auto palette = parse_palette(yaml_scene);
    if(palette.has_value()) {
        scene.palette = std::move(palette).value();
    }

    parse_objects(yaml_scene, scene.points, scene.lines, scene.rects);

    return scene;
}

