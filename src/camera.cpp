#include "camera.hpp"

#include <optional>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include "constants.hpp"
#include "math.hpp"
#include "renderer.hpp"

void write_camera_fov_to_buffers(Renderer& renderer, const Camera& camera) {
    static constexpr float ray_length = 10.0f;

    glm::vec3 dir = glm::normalize(glm::vec3(camera.direction.x, camera.direction.y, 0.0f)) * ray_length;
    float rotation_amount = glm::radians(camera.fov) / SCREEN_WIDTH;

    for(int i = SCREEN_WIDTH / 2; i > -SCREEN_WIDTH / 2; i--) {
        glm::vec3 v = glm::rotate(dir, i * rotation_amount, glm::vec3(0.0f, 0.0f, 1.0f));
        renderer.line_data[renderer.line_count] = glm::vec4(camera.position.x, camera.position.y, camera.position.x + v.x, camera.position.y + v.y);
        renderer.line_count++;
    }
}

void raycast_fov(Renderer& renderer, const glm::vec2& direction) {
    float max_distance = 2.5f;
    std::vector<glm::vec4> lines;
    lines.reserve(renderer.rect_count * 4);

    for(int i = 0; i < renderer.rect_count; i++) {
        glm::vec2 v0 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[0];
        glm::vec2 v1 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[1];
        glm::vec2 v2 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[2];
        glm::vec2 v3 = glm::vec2(renderer.positions[i].x, renderer.positions[i].y) + renderer.vertices[3];

        lines.emplace_back(glm::vec4(v0.x, v0.y, v1.x, v1.y));
        lines.emplace_back(glm::vec4(v1.x, v1.y, v3.x, v3.y));
        lines.emplace_back(glm::vec4(v3.x, v3.y, v2.x, v2.y));
        lines.emplace_back(glm::vec4(v2.x, v2.y, v0.x, v0.y));
    }

    for(int i = 0; i < renderer.line_data.size(); i++) {
        glm::vec2 start(renderer.line_data[i].x, renderer.line_data[i].y);
        glm::vec2 end(renderer.line_data[i].z, renderer.line_data[i].w);
        glm::vec2 dir = end - start;

        float closest_t = INFINITY;
        glm::vec4 color;
        for(int j = 0; j < lines.size(); j++) {
            std::optional<float> t = raycast(renderer.line_data[i], lines[j]);
            if(t.has_value() && t.value() < closest_t) {
                closest_t = t.value();
                color = renderer.colors[j / 4];
            }
        }

        if(closest_t != INFINITY) {
            glm::vec4 v = renderer.line_data[i];
            glm::vec2 ray_direction = glm::normalize(glm::vec2(v.z - v.x, v.w - v.y));
            float ray_angle = atan2(ray_direction.y, ray_direction.x);
            float player_angle = atan2(direction.y, direction.x);
            float corrected_distance = closest_t * cos(ray_angle - player_angle);

            glm::vec2 point = start + (end - start) * closest_t;
            renderer.line_data[i] = glm::vec4(start.x, start.y, point.x, point.y); 
            renderer.view_data[i] = glm::vec4(
                glm::clamp(max_distance * corrected_distance, 0.0f, 1.0f),
                color.x, color.y, color.z
            );
        }
    }
}

