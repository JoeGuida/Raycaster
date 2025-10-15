#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include <expected>
#include <string>

#include <Windows.h>

#include "input.hpp"

struct Window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, 
                                                    int width, int height, 
                                                    const wchar_t* class_name, const wchar_t* window_title,
                                                    Input& input);

#endif

