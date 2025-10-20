#include <array>
#include <cfloat>
#include <cstdio>
#include <expected>
#include <format>
#include <optional>
#include <print>
#include <unordered_map>

#include <Windows.h>
#include <gl/GL.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <opengl/glcorearb.h>
#include <spdlog/spdlog.h>

#include "camera.hpp"
#include "constants.hpp"
#include "convert.hpp"
#include "environment.hpp"
#include "gl_loader.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "map.hpp"
#include "math.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "window.hpp"

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    Scene scene {
        .background_color = glm::vec3(0.2157f, 0.1843f, 0.2275f)
    };

    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

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
    renderer.scene_data = scene;

    Camera camera {
        .position = glm::vec4(),
        .direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f),
        .fov = 45.0f,
        .aspect = width / static_cast<float>(height)
    };

    // pick and load a random map
    spdlog::info("loading map");
    Map map;
    std::string map_filepath = std::format("{}/map.txt", env_vars.value()["MAP_PATH"]);
    load_map_from_file(map, map_filepath);
    write_map_to_buffers(map, renderer, camera.aspect, color_palette);

    renderer.positions[renderer.rect_count] = glm::vec4(0.0f);
    renderer.colors[renderer.rect_count] = to_vec4(color_palette[8]);

    spdlog::info("setting up renderer to draw");
    glm::vec2 rect_size(camera.aspect / map.width, 2.0f / map.height);
    camera.position = glm::vec4(-camera.aspect + rect_size.x * 3.0f, 0.8f, 0.0f, 0.0f);
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

    glm::mat4 projection = glm::ortho(-camera.aspect, camera.aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    spdlog::info("setting shader uniforms");

    for(auto shader : renderer.shaders) { 
        glUseProgram(shader);
        set_shader_uniform(shader, "projection", projection);
        set_shader_uniform(shader, "view", view);
    }

    glUseProgram(shaders.value()["point"]);
    set_shader_uniform(shaders.value()["point"], "size", 3.0f);

    renderer.positions[POINT_OFFSET + renderer.point_count] = to_vec4(camera.position);
    renderer.colors[POINT_OFFSET + renderer.point_count] = to_vec4(camera.direction);
    renderer.point_count++;

    write_camera_fov_to_buffers(renderer, camera);
    raycast_fov(renderer, camera.direction);

    spdlog::info("starting message loop");

    renderer.view_count = width;
    setup(renderer);
    run_message_loop(window.value(), renderer, renderer.scene_data.background_color);

    return 0;
}

