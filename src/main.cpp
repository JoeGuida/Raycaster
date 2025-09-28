#include <array>
#include <print>
#include <unordered_map>

#include <Windows.h>
#include <gl/GL.h>

#include "renderer.hpp"
#include "window.hpp"

constexpr int width = 1280;
constexpr int height = 720;

std::unordered_map<std::string, std::string> load_environment_variables(const std::array<std::string, 2>& variable_names) {
    std::unordered_map<std::string, std::string> env_vars;
    for(auto& var : variable_names) {
        const char* env_var = std::getenv(var.c_str());
        if(!env_var) {
            std::println("Error: {} not set!", var);
        }

        env_vars[var] = std::string(env_var);
    }

    return env_vars;
}

int WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR command_line, int show_window_flags) {
    auto env_vars = load_environment_variables({"SHADER_PATH", "MAP_PATH"});

    Window window;
    Renderer renderer;
    if(!initialize_window(window, renderer, instance, show_window_flags, width, height, L"Window Class", L"Raycaster")) {
        std::println("window initialization failed");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    run_message_loop(window);

    return 0;
}

