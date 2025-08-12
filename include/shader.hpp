#ifndef RAYCASTER_SHADER_HPP
#define RAYCASTER_SHADER_HPP

#include <gl/GL.h>

#include <cstdint>
#include <format>
#include <iostream>
#include <string>

struct Shader {
	uint32_t id;
};

GLuint compile_shader(const std::string_view& src, GLenum type) {
	uint32_t shader = glCreateShader(type);
    const char* s = src.data();
	glShaderSource(shader, 1, &s, nullptr);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::string log(logLength, ' ');
		glGetShaderInfoLog(shader, logLength, nullptr, &log[0]);

		std::cerr << std::format("Vertex Shader Compilation Failed! :: {} {}", type, log) << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
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

#endif
