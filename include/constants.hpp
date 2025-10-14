#ifndef RAYCASTER_CONSTANTS_HPP
#define RAYCASTER_CONSTANTS_HPP

#include <cstdint>

constexpr size_t RECT_VERTICES_COUNT = 4;
constexpr size_t LINE_VERTICES_COUNT = 2;
constexpr size_t POINT_VERTICES_COUNT = 1;

constexpr size_t RECT_INDICES_COUNT = 6;

constexpr size_t OBJECT_COUNT_PER_PRIMITIVE = 256; // supported instances per primitive type
constexpr size_t MAX_OBJECTS = 256 * 2; // does not include lines as lines need separate data
constexpr size_t MAX_LINES = 2048;
constexpr size_t TOTAL_VERTICES_COUNT = RECT_VERTICES_COUNT + LINE_VERTICES_COUNT + POINT_VERTICES_COUNT; // Rect / Line / Point
constexpr uint32_t POINT_OFFSET = OBJECT_COUNT_PER_PRIMITIVE; // ubo index = gl_InstanceId + OFFSET

#endif
