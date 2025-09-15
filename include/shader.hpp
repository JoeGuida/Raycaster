#ifndef RAYCASTER_SHADER_HPP
#define RAYCASTER_SHADER_HPP

#include <Windows.h>
#include <gl/GL.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <cstdint>
#include <expected>
#include <string>

std::expected<uint32_t, std::string> compile_shader(const std::string& source_folder, const std::string& name, GLenum type);
std::expected<uint32_t, std::string> link_shaders(uint32_t vertex_shader, uint32_t fragment_shader);
void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::mat4x4& value);
void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::vec3& value);

#endif
