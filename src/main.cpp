#include <Windows.h>
#include <gl/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
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
#include <iostream>
#include <string>

HWND hwnd;
HGLRC hglrc;

uint32_t vao;
uint32_t vbo;
uint32_t ebo;

bool is_running = true;

constexpr float minimum_x_value = -1.0f;
constexpr float maximum_x_value = 1.0f;
constexpr float minimum_y_value = -1.0f;
constexpr float maximum_y_value = 1.0f;
constexpr float x_range = 2.0f;
constexpr float y_range = 2.0f;

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const char* shader_path = std::getenv("SHADER_PATH");
    if(!shader_path) {
        std::cerr << "Error: SHADER_PATH not set!" << std::endl;
        return -1;
    }

    const char* map_path = std::getenv("MAP_PATH");
    if(!map_path) {
        std::cerr << "Error: MAP_PATH not set!" << std::endl;
        return -1;
    }

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

    Renderer renderer;
    initialize_buffers(renderer);

    Map map;
    load_map_from_file(map, std::string(map_path) + "/map.txt");

    Rect rectangles;

    size_t non_space_tiles = std::count_if(map.data.begin(), map.data.end(), [](char c){ return c != ' '; });
    rectangles.positions.reserve(non_space_tiles);

    const glm::vec3 rect_size(x_range / map.width, y_range / map.height, 0.0f);
    const glm::vec3 half_size(rect_size.x / 2.0f, rect_size.y / 2.0f, 0.0f);
    rectangles.vertices = { 
        -half_size.x,  half_size.y, 0.0f,
         half_size.x,  half_size.y, 0.0f,
        -half_size.x, -half_size.y, 0.0f,
         half_size.x, -half_size.y, 0.0f
    };

    for(int i = 0; i < map.data.length(); i++) {
        if(map[i] == ' ') { continue; }

        float x_index = i % map.width;
        float y_index = i / map.width;

        float x = minimum_x_value + (rect_size.x / 2.0f) + x_index * rect_size.x; 
        float y = maximum_y_value - (rect_size.y / 2.0f) - y_index * rect_size.y; 

        rectangles.positions.push_back(glm::vec4(x, y, 0.0f, 0.0f)); 
    }

    setup_buffers(renderer);
    setup_instanced_elements(renderer, rectangles.vertices, rectangles.indices, rectangles.positions, rectangles.indices.size(), rectangles.positions.size());

    uint32_t vertex_shader = compile_shader(std::string(shader_path) + "/default.vert", GL_VERTEX_SHADER);
    uint32_t fragment_shader = compile_shader(std::string(shader_path) + "/default.frag", GL_FRAGMENT_SHADER);
    uint32_t shader_program = link_shaders(vertex_shader, fragment_shader);
    glUseProgram(shader_program);

    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.001, 100.0);
    set_shader_uniform(shader_program, "projection", projection);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));
    set_shader_uniform(shader_program, "view", view);
    
    loop_until_quit(window, renderer);

    std::cin.get();
    return 0;
}
