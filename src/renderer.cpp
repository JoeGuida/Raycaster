#include "renderer.hpp"

void render_update(uint32_t id) {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    constexpr std::array<uint32_t, 6> indices = {
        0, 1, 2, 
        1, 3, 2
    };

    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    for(int i = 0; i < rectangles.positions.size(); i++) {
        const glm::vec3 half_size = rectangles.sizes[i] / 2.0f;
        const glm::vec3 pos = rectangles.positions[i];
        std::array data = {
            -half_size.x, +half_size.y, 0.0f,
            +half_size.x, +half_size.y, 0.0f,
            -half_size.x, -half_size.y, 0.0f,
            +half_size.x, -half_size.y, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        
        glUseProgram(shader_program);
        set_shader_uniform(shader_program, "color", glm::vec3(0.25f, 0.0f, 0.0f));
        glm::mat4 model(1.0f);
        model = glm::translate(model, rectangles.positions[i]);
        set_shader_uniform(shader_program, "model", model);

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void setup_rect() {
    glBindVertexArray(vao);
    glUseProgram(shader_program);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
}
