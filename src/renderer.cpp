#include "renderer.hpp"

#include <Windows.h>

#include "gl_loader.hpp"

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_KEYDOWN: {
            if (wparam == VK_ESCAPE) {
                DestroyWindow(hwnd);
            }
            return 0; 
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

void initialize_buffers(Renderer& renderer) {
    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ebo);
    glGenBuffers(1, &renderer.ubo);
}

void draw(Renderer& renderer, size_t count) {
    glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, renderer.indices.data(), count / 6);
}

