#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include "renderer.hpp"

#include <cstdint>
#include <Windows.h>

struct Window {
    HGLRC hglrc;
    HDC hdc;
    HWND hwnd;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool initialize_window(Window& window, HINSTANCE hInstance, int ShowWnd, int width, int height, const wchar_t* windowName, const wchar_t* windowTitle); 
void loop_until_quit(Window& window, Renderer& renderer); 

#endif
