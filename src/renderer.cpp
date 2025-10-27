#include "renderer.hpp"

#include <Windows.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include "gl_loader.hpp"

void run_message_loop(Window& window, Renderer& renderer, const glm::vec3& background_color) {
    MSG message;
    ZeroMemory(&message, sizeof(MSG));
    while (true) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else {
            RECT client_rect;
            GetClientRect(window.hwnd, &client_rect);

            int client_width = client_rect.right - client_rect.left;
            int client_height = client_rect.bottom - client_rect.top;

            glViewport(0, 0, client_width, client_height);
            glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(renderer.vao);
            draw(renderer); 

            SwapBuffers(window.hdc);
        }
    }
}

void enable(const Renderer& renderer, const std::vector<uint32_t>& features) {
    for(const auto& feature : features) {
        glEnable(feature);
    }
}

void initialize(Renderer& renderer) {
    renderer.rect_count = 0;
    renderer.line_count = 0;
    renderer.point_count = 0;
    renderer.view_count = 0;

    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ebo);
    glGenBuffers(1, &renderer.rect_ubo);
    glGenBuffers(1, &renderer.line_ubo);
    glGenBuffers(1, &renderer.view_ubo);
}

void draw(Renderer& renderer) {
    glUseProgram(renderer.shaders["point"]);
    glDrawArraysInstanced(GL_POINTS, POINT_INDEX, 1, renderer.point_count);
    glUseProgram(renderer.shaders["line"]);
    glDrawArraysInstanced(GL_LINES, LINE_INDEX, 2, renderer.line_count);
    glUseProgram(renderer.shaders["view"]);
    glDrawArraysInstanced(GL_LINES, LINE_INDEX, 2, renderer.view_count);
    glUseProgram(renderer.shaders["rect"]);
    glDrawElementsInstanced(GL_TRIANGLES, renderer.indices.size(), GL_UNSIGNED_INT, nullptr, renderer.rect_count);
}

void setup(Renderer& renderer) {
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer.vertices.size() * sizeof(glm::vec2), renderer.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderer.indices.size() * sizeof(uint32_t), renderer.indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, renderer.rect_ubo);
    glBufferData(GL_UNIFORM_BUFFER, RECT_UBO_SIZE, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, POSITIONS_SIZE, renderer.positions.data());
    glBufferSubData(GL_UNIFORM_BUFFER, POSITIONS_SIZE, COLORS_SIZE, renderer.colors.data());
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, renderer.rect_ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, renderer.line_ubo);
    glBufferData(GL_UNIFORM_BUFFER, LINE_UBO_SIZE, renderer.line_data.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, renderer.line_ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, renderer.view_ubo);
    glBufferData(GL_UNIFORM_BUFFER, VIEW_UBO_SIZE, renderer.view_data.data(), GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, renderer.view_ubo);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);
}

