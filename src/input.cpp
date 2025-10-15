#include "input.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include <Windows.h>
#include <hidsdi.h>
#include <SetupAPI.h>

#include <spdlog/spdlog.h>
#include <winuser.h>

void remap(KeyCode keycode, ScanCode scancode) {
    keybindings[scancode] = keycode; 
}

std::string keycode_to_string(KeyCode keycode) {
    return keycode_strings[keycode];
}

std::string scancode_to_string(ScanCode scancode) {
    if(!keybindings.contains(scancode)) {
        return "Undefined";
    }

    return keycode_to_string(keybindings[scancode]);
}

void setup_input_devices(Input& input, HWND hwnd) {
    // KeyBoard
    input.devices[0] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x06,     // HID_USAGE_GENERIC_KEYBOARD
        .dwFlags = RIDEV_INPUTSINK,
        .hwndTarget = hwnd
    };

    // Mouse 
    input.devices[1] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x02,     // HID_USAGE_GENERIC_MOUSE
        .dwFlags = RIDEV_INPUTSINK,
        .hwndTarget = hwnd
    };

    if(RegisterRawInputDevices(input.devices.data(), input.devices.size(), sizeof(RAWINPUTDEVICE)) == FALSE) {
        spdlog::error("RegisterRawInputDevices failed!"); 
    }
}

void handle_inputs(LPARAM lparam, HWND hwnd) {
    UINT dw_size;
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dw_size];

    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
        spdlog::error("GetRawInputData returning incorrect size");
    }

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if(!raw) {
        spdlog::error("RAWINPUT* raw is null");
    }

    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        if((raw->data.keyboard.Flags & RI_KEY_BREAK) == 0) {
            spdlog::info("KeyDown: {}", scancode_to_string(static_cast<ScanCode>(raw->data.keyboard.MakeCode)));
            if(static_cast<ScanCode>(raw->data.keyboard.MakeCode) == ScanCode::Escape) {
                DestroyWindow(hwnd);
            }
        }
    }

    if(raw->header.dwType == RIM_TYPEMOUSE) {
        if(raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
            spdlog::info("MouseRel ({}, {})", raw->data.mouse.lLastX, raw->data.mouse.lLastY); 
        }
    }

    delete[] lpb;
}

