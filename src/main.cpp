#include <array>
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
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

constexpr int width = 1280;
constexpr int height = 720;

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    try {
        auto logger = spdlog::basic_logger_mt("logger", "log/log.log", true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::println("logger init failed: {}", ex.what());
        return -1;
    }

    Window window;
    Renderer renderer;
    spdlog::info("initializing window");
    if(!initialize_window(window, renderer, instance, show_window_flags, width, height, L"Window Class", L"Raycaster")) {
        spdlog::error("window initialization failed");
        return -1;
    }

    spdlog::info("logger: 'logger' initialized");
    spdlog::info("reading environment variables");
    auto env_vars = load_environment_variables({"SHADER_PATH", "MAP_PATH"});
    if(!env_vars.has_value()) {
        spdlog::error("{}", env_vars.error());
        return -1;
    }

    spdlog::info("compiling shaders");
    auto shaders = compile_shaders({"default"}, env_vars.value()["SHADER_PATH"]);
    if(!shaders.has_value()) {
        spdlog::info("{}", shaders.error());
        return -1;
    }

    initialize_buffers(renderer);
    renderer.vertices = { 
        glm::vec2(-0.1f,  0.1f), 
        glm::vec2( 0.1f,  0.1f),
        glm::vec2(-0.1f, -0.1f),
        glm::vec2( 0.1f, -0.1f)
    };

    renderer.indices = { 0, 1, 2, 1, 3, 2 };
    renderer.positions[0] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    renderer.count = 1;
    setup(renderer);

    //glEnable(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(shaders.value()["default"]);

    float aspect = width / static_cast<float>(height);
    glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 10.0f);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 1.0f), 
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));

    set_shader_uniform(shaders.value()["default"], "projection", projection);
    set_shader_uniform(shaders.value()["default"], "view", view);

    spdlog::info("starting message loop");
    run_message_loop(window, renderer);

    return 0;
}

