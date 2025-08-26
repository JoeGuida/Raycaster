#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include <cstdint>
#include <Windows.h>

struct Window {
    HGLRC hglrc;
    HDC hdc;
    HWND hwnd;
};

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool initialize_window(HWND hwnd, HINSTANCE hInstance, int ShowWnd, int width, int height, const wchar_t* windowName, const wchar_t* windowTitle);
void loop_until_quit(HWND hwnd, uint32_t renderer);

#endif
