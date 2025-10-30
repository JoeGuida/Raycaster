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
    float rotation_amount = camera.fov / SCREEN_WIDTH;

    for(int i = SCREEN_WIDTH / 2; i > -SCREEN_WIDTH / 2; i--) {
        glm::vec3 v = glm::rotate(dir, i * rotation_amount, glm::vec3(0.0f, 0.0f, 1.0f));
        renderer.line_data[renderer.line_count] = glm::vec4(camera.position.x, camera.position.y, camera.position.x + v.x, camera.position.y + v.y);
        renderer.line_count++;
    }
}

void raycast_fov(Renderer& renderer, const Camera& camera) {
    for(int i = 0; i < renderer.line_data.size(); i++) {
        glm::vec2 start(renderer.line_data[i].x, renderer.line_data[i].y);
        glm::vec2 end(renderer.line_data[i].z, renderer.line_data[i].w);

        float closest_t = INFINITY;
        glm::vec4 color;
        for(int j = 0; j < renderer.rect_lines.size(); j++) {
            std::optional<float> t = raycast(renderer.line_data[i], renderer.rect_lines[j]);
            if(t.has_value() && t.value() < closest_t) {
                closest_t = t.value();
                color = renderer.colors[j / 4];
            }
        }

        if(closest_t != INFINITY) {
            glm::vec4 v = renderer.line_data[i];
            glm::vec2 ray_direction = glm::normalize(glm::vec2(v.z - v.x, v.w - v.y));
            float scaled_t = closest_t * glm::length(end - start);
            float ray_angle = atan2(ray_direction.y, ray_direction.x);
            float player_angle = atan2(camera.direction.y, camera.direction.x);
            float corrected_distance = scaled_t * cos(ray_angle - player_angle);
            float projection_distance = (camera.aspect / 2.0f) / tan(camera.fov / 2.0f);

            float wall_height = 0.3f;
            float final_distance = wall_height * projection_distance / corrected_distance;

            glm::vec2 point = start + (end - start) * closest_t;
            renderer.line_data[i] = glm::vec4(start.x, start.y, point.x, point.y); 
            renderer.view_data[i] = glm::vec4(
                final_distance,
                color.x, color.y, color.z
            );
        }
    }
}

