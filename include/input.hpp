#ifndef RAYCASTER_INPUT_HPP
#define RAYCASTER_INPUT_HPP

#include <array>
#include <string>
#include <unordered_map>

#include <Windows.h>

enum class ScanCode : USHORT {
    A = 30,
    B = 48,
    C = 46,
    D = 32,
    E = 18,
    F = 33,
    G = 34,
    H = 35,
    I = 23,
    J = 36,
    K = 37,
    L = 38,
    M = 50,
    N = 49,
    O = 24,
    P = 25,
    Q = 16,
    R = 19,
    S = 31,
    T = 20,
    U = 22,
    V = 47,
    W = 17,
    X = 45,
    Y = 21,
    Z = 44,
    Escape = 1,
    Undefined = 0
};

enum class KeyCode : USHORT {
    A = 0,
    B = 1,
    C = 2,
    D = 3,
    E = 4,
    F = 5,
    G = 6,
    H = 7,
    I = 8,
    J = 9,
    K = 10,
    L = 11,
    M = 12,
    N = 13,
    O = 14,
    P = 15,
    Q = 16,
    R = 17,
    S = 18,
    T = 19,
    U = 20,
    V = 21,
    W = 22,
    X = 23,
    Y = 24,
    Z = 25,
    Escape = 26,
    Undefined = 27
};

static std::unordered_map<ScanCode, KeyCode> keybindings {
    { ScanCode::A, KeyCode::A },
    { ScanCode::B, KeyCode::B },
    { ScanCode::C, KeyCode::C },
    { ScanCode::D, KeyCode::D },
    { ScanCode::E, KeyCode::E },
    { ScanCode::F, KeyCode::F },
    { ScanCode::G, KeyCode::G },
    { ScanCode::H, KeyCode::H },
    { ScanCode::I, KeyCode::I },
    { ScanCode::J, KeyCode::J },
    { ScanCode::K, KeyCode::K },
    { ScanCode::L, KeyCode::L },
    { ScanCode::M, KeyCode::M },
    { ScanCode::N, KeyCode::N },
    { ScanCode::O, KeyCode::O },
    { ScanCode::P, KeyCode::P },
    { ScanCode::Q, KeyCode::Q },
    { ScanCode::R, KeyCode::R },
    { ScanCode::S, KeyCode::S },
    { ScanCode::T, KeyCode::T },
    { ScanCode::U, KeyCode::U },
    { ScanCode::V, KeyCode::V },
    { ScanCode::W, KeyCode::W },
    { ScanCode::X, KeyCode::X },
    { ScanCode::Y, KeyCode::Y },
    { ScanCode::Z, KeyCode::Z },
    { ScanCode::Escape, KeyCode::Escape },
    { ScanCode::Undefined, KeyCode::Undefined }
};

static std::unordered_map<KeyCode, std::string> keycode_strings {
    { KeyCode::A, "A" },
    { KeyCode::B, "B" },
    { KeyCode::C, "C" },
    { KeyCode::D, "D" },
    { KeyCode::E, "E" },
    { KeyCode::F, "F" },
    { KeyCode::G, "G" },
    { KeyCode::H, "H" },
    { KeyCode::I, "I" },
    { KeyCode::J, "J" },
    { KeyCode::K, "K" },
    { KeyCode::L, "L" },
    { KeyCode::M, "M" },
    { KeyCode::N, "N" },
    { KeyCode::O, "O" },
    { KeyCode::P, "P" },
    { KeyCode::Q, "Q" },
    { KeyCode::R, "R" },
    { KeyCode::S, "S" },
    { KeyCode::T, "T" },
    { KeyCode::U, "U" },
    { KeyCode::V, "V" },
    { KeyCode::W, "W" },
    { KeyCode::X, "X" },
    { KeyCode::Y, "Y" },
    { KeyCode::Z, "Z" },
    { KeyCode::Escape, "Escape" },
    { KeyCode::Undefined, "Undefined" }
};

struct Input {
    std::array<RAWINPUTDEVICE, 2> devices;
    bool initialized;
};

void remap(KeyCode keycode, ScanCode scancode);
std::string keycode_to_string(KeyCode keycode);
std::string scancode_to_string(ScanCode scancode);
void setup_input_devices(Input& input, HWND hwnd);
void handle_inputs(LPARAM lparam, HWND hwnd);

#endif
