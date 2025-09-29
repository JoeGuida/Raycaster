#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include <Windows.h>

struct Window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
};

#endif

