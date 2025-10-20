#ifndef RAYCASTER_MAIN_HPP
#define RAYCASTER_MAIN_HPP

#include <array>
#include <cfloat>
#include <cstdio>
#include <expected>
#include <filesystem>
#include <format>
#include <optional>
#include <print>
#include <unordered_map>

#include <Windows.h>
#include <gl/GL.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <opengl/glcorearb.h>
#include <spdlog/spdlog.h>

#include "camera.hpp"
#include "constants.hpp"
#include "convert.hpp"
#include "environment.hpp"
#include "gl_loader.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "map.hpp"
#include "math.hpp"
#include "renderer.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "window.hpp"

#endif
