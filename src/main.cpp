#include <Windows.h>
#include <gl/GL.h> 
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl_loader.hpp"
#include "map.hpp"
#include "rect.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <algorithm>
#include <array>
#include <print>
#include <string>
#include <unordered_map>

constexpr float minimum_x_value = -1.0f;
constexpr float maximum_x_value = 1.0f;
constexpr float minimum_y_value = -1.0f;
constexpr float maximum_y_value = 1.0f;
constexpr float x_range = 2.0f;
constexpr float y_range = 2.0f;

std::array colors = {
    glm::vec4(0.216f, 0.212f, 0.310f, 0.0f), // #37364e
    glm::vec4(0.208f, 0.364f, 0.410f, 0.0f), // #355d69
    glm::vec4(0.416f, 0.682f, 0.618f, 0.0f), // #6aae9d
    glm::vec4(0.725f, 0.835f, 0.710f, 0.0f), // #b9d4b4
    glm::vec4(0.957f, 0.914f, 0.835f, 0.0f), // #f4e9d4
    glm::vec4(0.816f, 0.729f, 0.663f, 0.0f), // #d0baa9
    glm::vec4(0.616f, 0.557f, 0.569f, 0.0f), // #9e8e91
    glm::vec4(0.357f, 0.290f, 0.408f, 0.0f)  // #5b4a68
};

std::vector<Rect> create_rectangles_from_map(const Map& map) {
    std::vector<Rect> rects;

    size_t non_space_tiles = std::count_if(map.data.begin(), map.data.end(), [](char c){ return c != ' '; });
    rects.reserve(non_space_tiles);

    const glm::vec3 rect_size(x_range / map.width, y_range / map.height, 0.0f);
    for(int i = 0; i < map.data.length(); i++) {
        if(map[i] == ' ') { continue; }

        float x_index = i % map.width;
        float y_index = i / map.width;

        float x = minimum_x_value + (rect_size.x / 2.0f) + x_index * rect_size.x;
        float y = maximum_y_value - (rect_size.y / 2.0f) - y_index * rect_size.y;

        rects.emplace_back(Rect(glm::vec4(x, y, 0.0f, 0.0f), Material(colors[map[i] - '0'])));
    }

    return rects;
}

std::unordered_map<std::string, std::string> load_environment_variables(const std::vector<const char*>& variable_names) {
    std::unordered_map<std::string, std::string> env_vars;
    for(auto& var : variable_names) {
        const char* env_var = std::getenv(var);
        if(!env_var) {
            std::println("Error: {} not set!", var);
        }

        env_vars[var] = std::string(env_var);
    }

    return env_vars;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    auto env_vars = load_environment_variables({"SHADER_PATH", "MAP_PATH"});

    // Initial Window Parameters
    constexpr wchar_t window_name[] = L"Default Window Class";
    constexpr wchar_t window_title[] = L"Raycaster";
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr double ASPECT_RATIO = width / static_cast<double>(height);
    
    Window window;
    if (!initialize_window(window, hInstance, nShowCmd, width, height, window_name, window_title)) {
        MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
        return 0;
    }

    // Compile Shaders
    uint32_t vertex_shader = compile_shader(env_vars["SHADER_PATH"] + "/default.vert", GL_VERTEX_SHADER);
    uint32_t point_shader = compile_shader(env_vars["SHADER_PATH"] + "/point.vert", GL_VERTEX_SHADER);
    uint32_t fragment_shader = compile_shader(env_vars["SHADER_PATH"] + "/default.frag", GL_FRAGMENT_SHADER);
    uint32_t rect_shader_program = link_shaders(vertex_shader, fragment_shader);
    uint32_t point_shader_program = link_shaders(point_shader, fragment_shader);

    Renderer renderer;
    initialize_buffers(renderer);
    setup_buffers(renderer);
    renderer.shaders["rect"] = rect_shader_program;
    renderer.shaders["point"] = point_shader_program;

    Map map;
    load_map_from_file(map, env_vars["MAP_PATH"] + "/map.txt");

    std::vector<Rect> rects = create_rectangles_from_map(map);
    setup_rects(renderer, rects);

    std::vector<Point> points = { Point(glm::vec3(0.0f), 100.0f, Material(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f))) };
    setup_points(renderer, points);

    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.001, 100.0);
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));


    glUseProgram(rect_shader_program);
    set_shader_uniform(rect_shader_program, "projection", projection);
    set_shader_uniform(rect_shader_program, "view", view);

    glUseProgram(point_shader_program);
    set_shader_uniform(point_shader_program, "projection", projection);
    set_shader_uniform(point_shader_program, "view", view);
    set_shader_uniform(point_shader_program, "color", points[0].material.color);
    set_shader_uniform(point_shader_program, "size", points[0].size);

    glUseProgram(rect_shader_program);
    loop_until_quit(window, renderer);

    return 0;
}
