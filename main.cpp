#define UNICODE

#include <Windows.h>
#include <gl/GL.h>
#include <glcorearb.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl_loader.hpp"
#include "shader.hpp"

#include <array>
#include <fstream>
#include <string_view>

HWND hwnd;
HGLRC hglrc;

uint32_t vao;
uint32_t vbo;
uint32_t ebo;

namespace rc {
    struct Rectangle {
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> sizes;
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
    HGLRC temp_context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp_context);

    LoadGLFunctions();

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(temp_context);
    wglMakeCurrent(hdc, hglrc);

    ShowWindow(hwnd, ShowWnd);
    UpdateWindow(hwnd);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void Update(HWND hwnd, GLuint shader_program, const rc::Rectangle& rectangles) {
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

            constexpr std::array<uint32_t, 6> indices = {
                0, 1, 2, 
                0, 2, 3
            };

            glBindVertexArray(vao);
            glUseProgram(shader_program);

            for(int i = 0; i < rectangles.positions.size(); i++) {
                const glm::vec3 half_size = rectangles.sizes[i] / 2.0f;
                const glm::vec3 pos = rectangles.positions[i];
                std::array data = {
                    pos.x - half_size.x, pos.y + half_size.y, 0.0f,
                    pos.x + half_size.x, pos.y + half_size.y, 0.0f,
                    pos.x + half_size.x, pos.y - half_size.y, 0.0f,
                    pos.x - half_size.x, pos.y - half_size.y, 0.0f
                };

                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                set_shader_uniform(shader_program, "color", glm::vec3(0.25f, 0.0f, 0.0f));
                glm::mat4 model(1.0f);
                model = glm::translate(model, rectangles.positions[i]);
                set_shader_uniform(shader_program, "model", model);

                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            }

            SwapBuffers(hdc);
        }
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    constexpr wchar_t window_name[] = L"Default Window Class";
    constexpr wchar_t window_title[] = L"Raycaster";
    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr double ASPECT_RATIO = width / static_cast<double>(height);
    
    if (!InitializeWindow(hInstance, nShowCmd, width, height, window_name, window_title)) {
        MessageBox(0, L"Window Initialization - Failed", L"Error", MB_OK);
        return 0;
    }

    LoadGLFunctions();

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

    rc::Rectangle rectangles;
    constexpr float size = 2.0f / 16;
    for(int i = 0; i < map.size(); i++) {
        if(map[i] == ' ') { continue; }
        glm::vec3 pos = glm::vec3(-1.0f + size / 2.0f, 1.0f - size / 2.0f, 0.0f);
        rectangles.positions.push_back(glm::vec3(
                    i % map_w * (2.0f / map_w), 
                    1.0f - i / map_h * (2.0f / map_h), 
                    0.0f));
        rectangles.sizes.push_back(glm::vec3(2.0f / map_w, 2.0f / map_h, 0.0f));
    }

    std::string shader_path = "C:/Users/JoeGu/source/repos/Raycaster/src/shaders/";
    GLuint vertex_shader = compile_shader(shader_path + "default.vert", GL_VERTEX_SHADER);
    GLuint fragment_shader = compile_shader(shader_path + "default.frag", GL_FRAGMENT_SHADER);
    GLuint shader_program = link_shaders(vertex_shader, fragment_shader);
    glUseProgram(shader_program);

    glm::mat4 projection = glm::ortho(-ASPECT_RATIO, ASPECT_RATIO, -1.0, 1.0, 0.001, 100.0);
    set_shader_uniform(shader_program, "projection", projection);
    
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f), 
        glm::vec3(0.0f, 0.0f, -1.0f), 
        glm::vec3(0.0f, 1.0f, 0.0f));
    set_shader_uniform(shader_program, "view", view);
    
    Update(hwnd, shader_program, rectangles);

    return 0;
}
