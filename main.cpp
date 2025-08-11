#define UNICODE
#define _UNICODE

#include <Windows.h>
#include <gl/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "gl_loader.hpp"
#include "shader.hpp"

#include <array>
#include <string_view>

HWND hwnd;
HGLRC hglrc;

uint32_t vao;
uint32_t vbo;
uint32_t ebo;

namespace Raycaster {
    struct Rectangle {
        std::vector<glm::vec3> positions;
        std::vector<float> sizes;
    };
};

bool is_running = true;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) {
            if (MessageBox(0, L"Are you sure you want to exit?", L"Exit", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                DestroyWindow(hwnd);
            }
        }
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, const wchar_t* windowName, const wchar_t* windowTitle) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = NULL;
    wc.cbWndExtra = NULL;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = windowName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, L"Error registering window class", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    hwnd = CreateWindowEx(NULL, windowName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
    if (!hwnd) {
        MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc, hglrc);

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    return true;
}

void Update(HWND hwnd, GLuint shader_program, const Raycaster::Rectangle& rectangles) {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    HDC hdc = GetDC(hwnd);
    while (is_running) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            constexpr std::array<uint32_t, 6> indices = {0, 1, 2, 0, 2, 3};

            glBindVertexArray(vao);
            glUseProgram(shader_program);

            for(int i = 0; i < rectangles.positions.size(); i++) {
                const float half_size = rectangles.sizes[i] / 2.0f;
                std::array data = {
                    -half_size,  half_size, 0.0f,
                     half_size,  half_size, 0.0f,
                     half_size, -half_size, 0.0f,
                    -half_size, -half_size, 0.0f
                };

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                glUniform3f(glGetUniformLocation(shader_program, "color"), 0.1f, 0.1f, 0.1f);
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            }

            glUniform3f(glGetUniformLocation(shader_program, "color"), 0.1f, 0.1f, 0.1f);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            SwapBuffers(hdc);
        }
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    constexpr wchar_t window_name[] = L"Default Window Class";
    constexpr wchar_t window_title[] = L"Raycaster";
    constexpr int width = 1280;
    constexpr int height = 720;
    
    if (!InitializeWindow(hInstance, nShowCmd, width, height, window_name, window_title)) {
        MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
        return 0;
    }

    LoadGLFunctions();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    
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

    Raycaster::Rectangle rectangles;
    for(char c : map) {
        rectangles.positions.push_back(glm::vec3(0.0f));
        rectangles.sizes.push_back(0.25f);
    }

    constexpr std::string_view vertex_shader_code = R"(
    #version 330 core

    layout(location = 0) in vec3 pos;

    uniform vec3 color;

    out vec3 vertex_color;

    void main() {
        gl_Position = vec4(pos, 1.0);
        vertex_color = color;
    }
    )";

    constexpr std::string_view fragment_shader_code = R"(
    #version 330 core

    in vec3 vertex_color;
    out vec4 frag_color;

    void main() {
        frag_color = vec4(vertex_color, 1.0);
    }
    )";

    Shader shader = {};
    GLuint vertex_shader = compile_shader(vertex_shader_code.data(), GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(fragment_shader_code.data(), GL_FRAGMENT_SHADER);
    GLuint shader_program = link_shaders(vertex_shader, fragment_shader);

    Update(hwnd, shader_program, rectangles);

    return 0;
}
