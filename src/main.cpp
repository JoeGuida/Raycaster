#define UNICODE

#include <Windows.h>
#include <gl/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl_loader.hpp"
#include "rect.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

#include <algorithm>
#include <array>
#include <iostream>

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
    // Attach a Console
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    // Initial Window Parameters
    constexpr wchar_t window_name[] = L"Default Window Class";
    constexpr wchar_t window_title[] = L"Raycaster";
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr double ASPECT_RATIO = width / static_cast<double>(height);
    
    if (!initialize_window(hInstance, nShowCmd, width, height, window_name, window_title)) {
        MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
        return 0;
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    constexpr int map_w = 16;
    constexpr int map_h = 16;
    constexpr std::string_view map = 
    "0000222222220000"\
    "1              0"\
    "1      11111   0"\
    "1     0        0"\
    "0     0  1110000"\
    "0     3        0"\
    "0   10000      0"\
    "0   0   11100  0"\
    "0   0   0      0"\
    "0   0   1  00000"\
    "0       1      0"\
    "2       1      0"\
    "0       0      0"\
    "0 0000000      0"\
    "0              0"\
    "0002222222200000";

    Rect rectangles;
    size_t non_space_tiles = std::count_if(map.begin(), map.end(), [](char c){ return c != ' '; });
    rectangles.vertices.reserve(non_space_tiles * 12);
    for(int i = 0; i < map.size(); i++) {
        if(map[i] == ' ') { continue; }

        const glm::vec3 rect_size(x_range / map_w, y_range / map_h, 0.0f);
        const glm::vec3 half_size(rect_size.x / 2.0f, rect_size.y / 2.0f, 0.0f);

        float x_index = i % map_w;
        float y_index = i / map_w;

        rectangles.positions.push_back(glm::vec3(
                    minimum_x_value + (rect_size.x / 2.0f) + x_index * rect_size.x, 
                    maximum_y_value - (rect_size.y / 2.0f) - y_index * rect_size.y, 
                    0.0f));
        rectangles.sizes.push_back(glm::vec3(rect_size.x, rect_size.y, 0.0f));
        const std::array<float, 12> vertices = { 
            -half_size.x,  half_size.y, 0.0f,
             half_size.x,  half_size.y, 0.0f,
            -half_size.x, -half_size.y, 0.0f,
             half_size.x, -half_size.y, 0.0f
        };

        rectangles.vertices.insert(rectangles.vertices.end(), vertices.begin(), vertices.end());
    }

    std::string shader_path = std::getenv("SHADER_PATH");    
    if(shader_path.empty()) {
        std::cerr << "Error: SHADER_PATH not set!" << std::endl; 
        return -1;
    }

    uint32_t vertex_shader = compile_shader(shader_path + "default.vert", GL_VERTEX_SHADER);
    uint32_t fragment_shader = compile_shader(shader_path + "default.frag", GL_FRAGMENT_SHADER);
    uint32_t shader_program = link_shaders(vertex_shader, fragment_shader);
    glUseProgram(shader_program);

    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.001, 100.0);
    set_shader_uniform(shader_program, "projection", projection);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));
    set_shader_uniform(shader_program, "view", view);
    
    Renderer renderer;
    loop_until_quit(hwnd, renderer.id);

    return 0;
}
