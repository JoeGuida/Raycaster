#ifndef RAYCASTER_WINDOW_HPP
#define RAYCASTER_WINDOW_HPP

#include "rect.hpp"

#include <cstdint>
#include <Windows.h>

LRESULT CALLBACK window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool initialize_window(HINSTANCE hInstance, int ShowWnd, int width, int height, const wchar_t* windowName, const wchar_t* windowTitle);
void loop_intil_quit(HWND hwnd, uint32_t shader_program, const Rect& rectangles);

#endif
