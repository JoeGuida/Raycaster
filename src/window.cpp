#include "window.hpp"

#include <Windows.h>
#include <GL/gl.h>
#include <glcorearb.h>

#include "gl_loader.hpp"
#include "renderer.hpp"

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

bool initialize_window(Window& window, HINSTANCE hInstance, int ShowWnd, int width, int height, const wchar_t* windowName, const wchar_t* windowTitle) {
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = window_proc;
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

    window.hwnd = CreateWindowEx(NULL, windowName, windowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);
    if (!window.hwnd) {
        MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }

    window.hdc = GetDC(window.hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(window.hdc, &pfd);
    SetPixelFormat(window.hdc, pixelFormat, &pfd);
    HGLRC temp_context = wglCreateContext(window.hdc);
    wglMakeCurrent(window.hdc, temp_context);

    LoadGLFunctions();

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    window.hglrc = wglCreateContextAttribsARB(window.hdc, 0, attribs);

    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(temp_context);
    wglMakeCurrent(window.hdc, window.hglrc);

    ShowWindow(window.hwnd, ShowWnd);
    UpdateWindow(window.hwnd);

    glEnable(GL_DEPTH_TEST);

    return true;
}

void loop_until_quit(Window& window, Renderer& renderer) {
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (true) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            render_update(renderer);
            SwapBuffers(window.hdc);
        }
    }
}
