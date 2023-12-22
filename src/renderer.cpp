#include "renderer.hpp"

uint32_t Renderer::VAO;
uint32_t Renderer::VBO;
uint32_t Renderer::EBO;

void Renderer::draw(const Line& line, const std::shared_ptr<Shader>& shader) {
    glm::vec3 start = line.start;
    glm::vec3 end = line.end;
    std::array<uint32_t, 2> indices{ 0, 1 };
    std::array<float, 6> vertices{
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // Apply matrix transformation and set shader uniforms
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, line.transform.position);
    model = glm::rotate(model, line.transform.rotation.z, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, line.transform.scale);
    shader->set("model", model);
    shader->set("color", line.material.color);

    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void Renderer::draw(const Point& point, const std::shared_ptr<Shader>& shader) {
    float vertex = 0.0f;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float), &vertex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // Apply matrix transformation and set shader uniforms
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, point.transform.position);
    model = glm::rotate(model, point.transform.rotation.z, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, point.transform.scale);
    shader->set("model", model);
    shader->set("color", point.material.color);
    shader->set("pointSize", point.size);

    glDrawArrays(GL_POINTS, 0, 1);
}

void Renderer::draw(const Ray& ray, const Material& material, const std::shared_ptr<Shader>& shader) {
    glm::vec3 destination = ray.origin + ray.direction * 4.0f;
    std::array<uint32_t, 2> indices{ 0, 1 };
    std::array<float, 6> vertices{
        0.0f, 0.0f, 0.0f,
        destination.x, destination.y, destination.z
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // Apply matrix transformation and set shader uniforms
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, ray.origin);
    shader->set("model", model);
    shader->set("color", material.color);

    glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
}

void Renderer::draw(const Rectangle& rectangle, const std::shared_ptr<Shader>& shader) {
    std::array<uint32_t, 6> indices { 
        0, 1, 2, 
        1, 2, 3 
    };
    std::array<float, 12> vertices {
        -rectangle.width / 2.0f,  rectangle.height / 2.0f, 0.0f,
         rectangle.width / 2.0f,  rectangle.height / 2.0f, 0.0f,
        -rectangle.width / 2.0f, -rectangle.height / 2.0f, 0.0f,
         rectangle.width / 2.0f, -rectangle.height / 2.0f, 0.0f
    }; 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);

    // Apply matrix transformation and set shader uniforms
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, rectangle.transform.position);
    model = glm::rotate(model, rectangle.transform.rotation.z, glm::vec3(0.0f, 0.0f, -1.0f));
    model = glm::scale(model, rectangle.transform.scale);
    shader->set("model", model);
    shader->set("color", rectangle.material.color);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}