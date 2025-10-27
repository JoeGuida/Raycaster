#ifndef RAYCASTER_CONSTANTS_HPP
#define RAYCASTER_CONSTANTS_HPP

#include <array>
#include <cstdint>

#include <glm/vec3.hpp>

constexpr int SCREEN_WIDTH  = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr size_t RECT_VERTICES_COUNT = 4;
constexpr size_t LINE_VERTICES_COUNT = 2;
constexpr size_t POINT_VERTICES_COUNT = 1;

constexpr size_t RECT_INDICES_COUNT = 6;

constexpr size_t OBJECT_COUNT_PER_PRIMITIVE = 256; // supported instances per primitive type
constexpr size_t MAX_OBJECTS = 256 * 2; // does not include lines as lines need separate data
constexpr size_t MAX_LINES = 2048;
constexpr size_t TOTAL_VERTICES_COUNT = RECT_VERTICES_COUNT + LINE_VERTICES_COUNT + POINT_VERTICES_COUNT; // Rect / Line / Point
constexpr uint32_t POINT_OFFSET = OBJECT_COUNT_PER_PRIMITIVE; // ubo index = gl_InstanceId + OFFSET

constexpr std::array<glm::vec3, 10> color_palette = {
    glm::vec3(0.2157f, 0.1843f, 0.2275f), // #372F3A
    glm::vec3(0.2745f, 0.2667f, 0.3490f), // #464459
    glm::vec3(0.3294f, 0.3686f, 0.4471f), // #545E72
    glm::vec3(0.3647f, 0.4627f, 0.5020f), // #5D7680
    glm::vec3(0.4157f, 0.5765f, 0.5843f), // #6A9395
    glm::vec3(0.4824f, 0.6784f, 0.6235f), // #7BAD9F
    glm::vec3(0.5569f, 0.6980f, 0.6039f), // #8EB29A
    glm::vec3(0.7020f, 0.7765f, 0.7059f), // #B3C6B4
    glm::vec3(0.7725f, 0.8235f, 0.8078f), // #C5D2CE
    glm::vec3(0.8275f, 0.8471f, 0.8510f)  // #D3D8D9
};

#endif
