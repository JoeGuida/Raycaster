#include <array>
#include <cfloat>
#include <cstdio>
#include <format>
#include <optional>
#include <print>
#include <unordered_map>

#include <Windows.h>
#include <gl/GL.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <opengl/glcorearb.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "environment.hpp"
#include "gl_loader.hpp"
#include "input.hpp"
#include "map.hpp"
#include "math.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

constexpr int width = 1280;
constexpr int height = 720;
constexpr float aspect = width / static_cast<float>(height);
constexpr float PI = 3.1415926;

// https://lospec.com/palette-list/dull-aquatic
constexpr std::array<glm::vec4, 10> color_palette = {
    glm::vec4(0.2157f, 0.1843f, 0.2275f, 1.0f),  // #372F3A
    glm::vec4(0.2745f, 0.2667f, 0.3490f, 1.0f),  // #464459
    glm::vec4(0.3294f, 0.3686f, 0.4471f, 1.0f),  // #545E72
    glm::vec4(0.3647f, 0.4627f, 0.5020f, 1.0f),  // #5D7680
    glm::vec4(0.4157f, 0.5765f, 0.5843f, 1.0f),  // #6A9395
    glm::vec4(0.4824f, 0.6784f, 0.6235f, 1.0f),  // #7BAD9F
    glm::vec4(0.5569f, 0.6980f, 0.6039f, 1.0f),  // #8EB29A
    glm::vec4(0.7020f, 0.7765f, 0.7059f, 1.0f),  // #B3C6B4
    glm::vec4(0.7725f, 0.8235f, 0.8078f, 1.0f),  // #C5D2CE
    glm::vec4(0.8275f, 0.8471f, 0.8510f, 1.0f)   // #D3D8D9
};

void write_player_fov_to_buffers(Renderer& renderer, const glm::vec4& position, const glm::vec4& direction) {
    static constexpr float fov = 45.0f;
    static constexpr float ray_length = 10.0f;
    glm::vec3 dir = glm::normalize(glm::vec3(direction.x, direction.y, 0.0f)) * ray_length;
    float rotation_amount = glm::radians(fov) / width;

    for(int i = width / 2; i > -width / 2; i--) {
        glm::vec3 v = glm::rotate(dir, i * rotation_amount, glm::vec3(0.0f, 0.0f, 1.0f));
        renderer.line_data[renderer.line_count] = glm::vec4(position.x, position.y, position.x + v.x, position.y + v.y);
        renderer.line_count++;
    }
}

void raycast_fov(Renderer& renderer, const glm::vec2& direction) {
    float max_distance = 2.5f;
    std::vector<glm::vec4> lines;
    lines.reserve(renderer.rect_count * 4);

    for(int i = 0; i < renderer.rect_count; i++) {
        glm::vec2 v0 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[0];
        glm::vec2 v1 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[1];
        glm::vec2 v2 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[2];
        glm::vec2 v3 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[3];

        lines.emplace_back(glm::vec4(v0.x, v0.y, v1.x, v1.y));
        lines.emplace_back(glm::vec4(v1.x, v1.y, v3.x, v3.y));
        lines.emplace_back(glm::vec4(v3.x, v3.y, v2.x, v2.y));
        lines.emplace_back(glm::vec4(v2.x, v2.y, v0.x, v0.y));
    }

    for(int i = 0; i < renderer.line_data.size(); i++) {
        glm::vec2 start(renderer.line_data[i].x, renderer.line_data[i].y);
        glm::vec2 end(renderer.line_data[i].z, renderer.line_data[i].w);
        glm::vec2 dir = end - start;

        float closest_t = INFINITY;
        glm::vec4 color;
        for(int j = 0; j < lines.size(); j++) {
            std::optional<float> t = raycast(renderer.line_data[i], lines[j]);
            if(t.has_value() && t.value() < closest_t) {
                closest_t = t.value();
                color = renderer.colors[j / 4];
            }
        }

        if(closest_t != INFINITY) {
            glm::vec4 v = renderer.line_data[i];
            glm::vec2 ray_direction = glm::normalize(glm::vec2(v.z - v.x, v.w - v.y));
            float ray_angle = atan2(ray_direction.y, ray_direction.x);
            float player_angle = atan2(direction.y, direction.x);
            float corrected_distance = closest_t * cos(ray_angle - player_angle);

            glm::vec2 point = start + (end - start) * closest_t;
            renderer.line_data[i] = glm::vec4(start.x, start.y, point.x, point.y); 
            renderer.view_data[i] = glm::vec4(
                glm::clamp(max_distance * corrected_distance, 0.0f, 1.0f),
                color.x, color.y, color.z
            );
        }
    }
}

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    std::fflush(stdout);
    try {
        auto logger = spdlog::basic_logger_mt("logger", "log/log.log", true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::println("logger init failed: {}", ex.what());
        std::fflush(stdout);
        return -1;
    }
    spdlog::info("logger: 'logger' initialized");

    spdlog::info("reading environment variables");
    auto env_vars = load_environment_variables({"SHADER_PATH", "MAP_PATH"});
    if(!env_vars.has_value()) {
        spdlog::error("{}", env_vars.error());
        return -1;
    }

    spdlog::info("initializing window");
    Input input { .initialized = false };
    auto window = initialize_window(instance, show_window_flags, width, height, L"Window Class", L"Raycaster", input);
    if(!window.has_value()) {
        spdlog::error("{}", window.error());
    }

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);

    // pick and load a random map
    spdlog::info("loading map");
    Map map;
    std::string map_filepath = std::format("{}/map.txt", env_vars.value()["MAP_PATH"]);
    load_map_from_file(map, map_filepath);
    write_map_to_buffers(map, renderer, aspect, color_palette);

    renderer.positions[renderer.rect_count] = glm::vec4(0.0f);
    renderer.colors[renderer.rect_count] = color_palette[8];

    spdlog::info("compiling shaders");
    auto shaders = compile_shaders({"rect", "line", "point", "view"}, env_vars.value()["SHADER_PATH"]);
    if(!shaders.has_value()) {
        spdlog::info("{}", shaders.error());
        return -1;
    }

    renderer.shaders = { 
        shaders.value()["rect"], 
        shaders.value()["line"], 
        shaders.value()["point"], 
        shaders.value()["view"] 
    };

    spdlog::info("setting up renderer to draw");
    glm::vec2 rect_size(aspect / map.width, 2.0f / map.height);
    float x = rect_size.x / 2.0f;
    float y = rect_size.y / 2.0f;
    renderer.vertices = { 
        // rect
        glm::vec2(-x,  y),
        glm::vec2( x,  y),
        glm::vec2(-x, -y),
        glm::vec2( x, -y),
        //line
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        // point
        glm::vec2(0.0f, 0.0f)
    };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    spdlog::info("setting shader uniforms");

    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));


    for(auto shader : renderer.shaders) { 
        glUseProgram(shader);
        set_shader_uniform(shader, "projection", projection);
        set_shader_uniform(shader, "view", view);
    }

    glUseProgram(shaders.value()["point"]);
    set_shader_uniform(shaders.value()["point"], "size", 3.0f);

    // Objects
    glm::vec4 player_position(-aspect + rect_size.x * 3.0f, 0.8f, 0.0f, 0.0f);
    glm::vec4 player_direction(0.0f, -1.0f, 0.0f, 0.0f);
    renderer.positions[POINT_OFFSET + renderer.point_count] = player_position;
    renderer.colors[POINT_OFFSET + renderer.point_count] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    renderer.point_count++;

    write_player_fov_to_buffers(renderer, player_position, player_direction);
    raycast_fov(renderer, player_direction);

    spdlog::info("starting message loop");

    renderer.view_count = width;
    setup(renderer);
    run_message_loop(window.value(), renderer);

    return 0;
}

