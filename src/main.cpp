#include <array>
#include <cstdio>
#include <format>
#include <print>
#include <unordered_map>

#include <Windows.h>
#include <gl/GL.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <opengl/glcorearb.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "environment.hpp"
#include "gl_loader.hpp"
#include "map.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

constexpr int width = 1280;
constexpr int height = 720;
constexpr float aspect = width / static_cast<float>(height);

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
    auto window = initialize_window(instance, show_window_flags, width, height, L"Window Class", L"Raycaster");
    if(!window.has_value()) {
        spdlog::error("{}", window.error());
    }

    spdlog::info("initializing renderer");
    Renderer renderer;
    initialize(renderer);

    spdlog::info("loading map");
    Map map;
    load_map_from_file(map, env_vars.value()["MAP_PATH"] + "/map.txt");
    write_map_to_buffers(map, renderer, aspect, color_palette);

    spdlog::info("compiling shaders");
    auto shaders = compile_shaders({"rect", "point"}, env_vars.value()["SHADER_PATH"]);
    if(!shaders.has_value()) {
        spdlog::info("{}", shaders.error());
        return -1;
    }

    renderer.rect_shader = shaders.value()["rect"];
    renderer.point_shader = shaders.value()["point"];

    spdlog::info("setting up renderer to draw");
    glm::vec2 rect_size(aspect / map.width, 2.0f / map.height);
    float x = rect_size.x / 2.0f;
    float y = rect_size.y / 2.0f;
    renderer.vertices = { 
        glm::vec2(-x,  y),
        glm::vec2( x,  y),
        glm::vec2(-x, -y),
        glm::vec2( x, -y)
    };

    renderer.indices = { 0, 2, 1, 1, 2, 3, 0, 1 };

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    spdlog::info("setting shader uniforms");

    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));


    glUseProgram(shaders.value()["rect"]);
    set_shader_uniform(shaders.value()["rect"], "projection", projection);
    set_shader_uniform(shaders.value()["rect"], "view", view);

    glUseProgram(shaders.value()["point"]);
    set_shader_uniform(shaders.value()["point"], "projection", projection);
    set_shader_uniform(shaders.value()["point"], "view", view);
    set_shader_uniform(shaders.value()["point"], "size", 10.0f);

    // Objects
    renderer.positions[POINT_OFFSET + renderer.point_count] = glm::vec4(0.0f);
    renderer.colors[POINT_OFFSET + renderer.point_count] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
    renderer.point_count++;

    spdlog::info("starting message loop");

    glUseProgram(shaders.value()["rect"]);
    setup(renderer);
    run_message_loop(window.value(), renderer);

    return 0;
}

