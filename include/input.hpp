#ifndef RAYCASTER_INPUT_HPP
#define RAYCASTER_INPUT_HPP

#include <array>

#include <Windows.h>

enum class KeyCode : USHORT {
    W = 17,
    A = 30,
    S = 31,
    D = 32,
    Escape = 1,
    Undefined = 0
};

struct Input {
    std::array<RAWINPUTDEVICE, 2> devices;
    bool initialized;
};

void setup_input_devices(Input& input, HWND hwnd);
void handle_inputs(LPARAM lparam, HWND hwnd);

#endif
