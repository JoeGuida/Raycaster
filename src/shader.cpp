#include "shader.hpp"
#include "gl_loader.hpp"

#include <Windows.h>
#include <gl/GL.h>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <print>

uint32_t compile_shader(const std::string& filepath, GLenum type) {
    std::ifstream shader_file(filepath);
    if(!shader_file) {
        std::println("ERROR READING FILE :: {}", filepath);
    }

    shader_file.seekg(0, std::ios::end);
    size_t size = shader_file.tellg();
    shader_file.seekg(0);
    std::string buffer(size, '\0');
    shader_file.read(&buffer[0], size);

    uint32_t id = glCreateShader(type);
    const char* s = buffer.data();
	glShaderSource(id, 1, &s, nullptr);
	glCompileShader(id);

	GLint success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint logLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
		std::string log(logLength, ' ');
		glGetShaderInfoLog(id, logLength, nullptr, &log[0]);

		std::println("Shader Compilation Failed! :: {} {}", type, log);
		glDeleteShader(id);
		return 0;
	}

	return id; 
}

uint32_t link_shaders(uint32_t vertex_shader, uint32_t fragment_shader) {
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		GLint logLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::string log(logLength, ' ');
		glGetProgramInfoLog(program, logLength, nullptr, &log[0]);

		std::println("Shader program linking Failed!");
		glDeleteProgram(program);
		return 0;
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::mat4x4& value) {
    glUniformMatrix4fv(glGetUniformLocation(program, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(program, uniform.c_str()), 1, glm::value_ptr(value));
}
