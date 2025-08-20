#ifndef RAYCASTER_SHADER_HPP
#define RAYCASTER_SHADER_HPP

#include <Windows.h>
#include <gl/GL.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

struct Shaders {
    std::vector<GLuint> ids;
};

GLuint compile_shader(const std::string& filepath, GLenum type); 
GLuint link_shaders(GLuint vertex_shader, GLuint fragment_shader);
void set_shader_uniform(GLuint program, const std::string& uniform, const glm::mat4x4& value);
void set_shader_uniform(GLuint program, const std::string& uniform, const glm::vec3& value);

#endif
