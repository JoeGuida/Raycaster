#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include <Windows.h>

#include "renderer.hpp"

struct Window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
};

bool initialize_window(Window& window, Renderer& renderer, HINSTANCE instance, int show_window_flags, int width, int height, 
                       const wchar_t* class_name, const wchar_t* window_title);
void run_message_loop(Window& window);

#endif

