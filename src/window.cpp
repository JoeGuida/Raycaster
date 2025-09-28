#include "window.hpp"

#include <print>

#include <Windows.h>

#include "gl_loader.hpp"
#include "renderer.hpp"

bool initialize_window(Window& window, Renderer& renderer, HINSTANCE instance, int show_window_flags, int width, int height, 
                       const wchar_t* class_name, const wchar_t* window_title) {
    WNDCLASSEX window_class {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_proc,
        .cbClsExtra = NULL,
        .cbWndExtra = NULL,
        .hInstance = instance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = nullptr,
        .lpszMenuName = NULL,
        .lpszClassName = class_name,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&window_class)) {
        std::println("error registering window class");
        return false;
    }

    window.hwnd = CreateWindowEx(NULL, class_name, window_title, WS_OVERLAPPEDWINDOW, 
                                 CW_USEDEFAULT, CW_USEDEFAULT, width, height, 
                                 NULL, NULL, instance, &renderer);

    if (!window.hwnd) {
        std::println("error creating window");
        return false;
    }

    window.hdc = GetDC(window.hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE
    };

    int pixel_format = ChoosePixelFormat(window.hdc, &pfd);
    SetPixelFormat(window.hdc, pixel_format, &pfd);
    HGLRC temp_context = wglCreateContext(window.hdc);
    wglMakeCurrent(window.hdc, temp_context);

    load_gl_functions();

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    window.hglrc = wglCreateContextAttribsARB(window.hdc, 0, attribs);
    if(window.hglrc) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_context);
        wglMakeCurrent(window.hdc, window.hglrc); 
    }

    ShowWindow(window.hwnd, show_window_flags);
    UpdateWindow(window.hwnd);

    return true;
}

void run_message_loop(Window& window) {
    MSG message;
    ZeroMemory(&message, sizeof(MSG));
    while (true) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else {
            glClearColor(0.498f, 0.498f, 0.498f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            SwapBuffers(window.hdc);
        }
    }
}

