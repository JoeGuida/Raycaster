#ifndef RAYCASTER_SHADER_HPP
#define RAYCASTER_SHADER_HPP

#include <gl/GL.h>

#include <glm/gtc/type_ptr.hpp>

#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <string>

struct Shader {
	uint32_t id;
};

GLuint compile_shader(const std::string& filepath, GLenum type) {
    std::ifstream shader_file(filepath);
    if(!shader_file) {
        std::cerr << std::format("ERROR READING FILE :: {}", filepath) << std::endl;
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

		std::cerr << std::format("Shader Compilation Failed! :: {} {}", type, log) << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id; 
}

GLuint link_shaders(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint program = glCreateProgram();
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

		std::cerr << "Shader program linking Failed!" << std::endl;
		glDeleteProgram(program);
		return 0;
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

void set_shader_uniform(GLuint program, const std::string& uniform, const glm::mat4x4& value) {
    glUniformMatrix4f(glGetUniformLocation(program, uniform.c_str()), glm::value_ptr(value));
}

#endif
