#include "shader.hpp"

#include <expected>
#include <filesystem>
#include <fstream>
#include <format>
#include <print>
#include <string>
#include <unordered_map>
#include <vector>

#include <Windows.h>

#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include "gl_loader.hpp"

std::expected<uint32_t, std::string> compile_shader(const std::string& source_folder, const std::string& name, uint32_t type) {
    std::filesystem::path filepath(source_folder);
    std::string extension;
    switch(type) {
        case GL_VERTEX_SHADER: {
            extension = ".vert";
            break;
        }
        case GL_FRAGMENT_SHADER: {
            extension = ".frag";
            break;
        }
        default: {
            return std::unexpected(std::format("ERROR :: {} is not a valid type for shader compilation", type)); 
        }
    }

    std::string shader_name = name + extension;
    filepath = filepath / shader_name;

    std::ifstream shader_file(filepath.string());
    if(!shader_file) {
        return std::unexpected(std::format("ERROR READING FILE :: {}", filepath.string()));
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

	int success;
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success) {
		int log_length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
		std::string log(log_length, ' ');
		glGetShaderInfoLog(id, log_length, nullptr, &log[0]);

		glDeleteShader(id);
		return std::unexpected(std::format("Shader Compilation Failed! :: {} {}", type, log));
	}

	return id; 
}

std::expected<uint32_t, std::string> link_shaders(uint32_t vertex_shader, uint32_t fragment_shader) {
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		int log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_length);

		std::string log(log_length, '\0');
		glGetProgramInfoLog(program, log_length, nullptr, log.data());

		glDeleteProgram(program);
	    glDeleteShader(vertex_shader);
	    glDeleteShader(fragment_shader);
		return std::unexpected(std::format("Shader program linking Failed! :: {}", log));
	}

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return program;
}

void set_shader_uniform(uint32_t program, const std::string& uniform, float value) {
    glUniform1f(glGetUniformLocation(program, uniform.c_str()), value);
}

void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::mat4x4& value) {
    glUniformMatrix4fv(glGetUniformLocation(program, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(program, uniform.c_str()), 1, glm::value_ptr(value));
}

std::expected<std::unordered_map<std::string, uint32_t>, std::string> compile_shaders(const std::vector<std::string>& shader_names, const std::string& path) {
    std::unordered_map<std::string, uint32_t> shaders;
    for(auto& name : shader_names) {
        auto vertex_shader = compile_shader(path, name, GL_VERTEX_SHADER);
        auto fragment_shader = compile_shader(path, name, GL_FRAGMENT_SHADER);

        if(!vertex_shader.has_value()) { 
            spdlog::info("vertex");
            return std::unexpected(std::format("{}", vertex_shader.error())); 
        }

        if(!fragment_shader.has_value()) { 
            spdlog::info("fragment");
            return std::unexpected(std::format("{}", fragment_shader.error())); 
        }

        auto program = link_shaders(vertex_shader.value(), fragment_shader.value());
        if(!program.has_value()) { 
            spdlog::info("program");
            return std::unexpected(std::format("{}", program.error())); 
        }

        shaders[name] = program.value();
    }

    return shaders;
}

