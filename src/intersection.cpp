#include "intersection.hpp"

float Intersection::raycast(const Ray& ray, const Line& line) {
    glm::vec3 v1 = ray.origin - line.start;
    glm::vec3 v2 = line.end - line.start;
    glm::vec3 v3(-ray.direction.y, ray.direction.x, 0.0f);

    float dot = glm::dot(v2, v3);
    if (abs(dot) < 0.000001f)
        return -1.0f;

    float cross = v2.x * v1.y - v2.y * v1.x;
    float t1 = cross / dot;
    float t2 = glm::dot(v1, v3) / dot;

    if (t1 >= 0.0f && (t2 >= 0.0f && t2 <= 1.0f))
        return t1;

    return -1.0f;
}
