#ifndef RAYCASTER_SHADER_HPP
#define RAYCASTER_SHADER_HPP

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Shader
{
public:
    Shader(const Shader&) = default;
    Shader(Shader&&) = default;
    virtual ~Shader() = default;
    Shader& operator=(const Shader& shader) = default;

    uint32_t getID() const { return ID; }
    void setID(uint32_t newID) { ID = newID; }
    void set(const char* uniformName, float value) const;
    void set(const char* uniformName, int value) const;
    void set(const char* uniformName, const glm::mat4& value) const;
    void set(const char* uniformName, const glm::vec3& value) const;
    void set(const char* uniformName, float x, float y, float z) const;
    void use() const;

    friend class ShaderFactory;

private:
    Shader() : ID(0) {}

    uint32_t ID;
};

class ShaderFactory {
public:
    static std::shared_ptr<Shader> create(const std::string& name);
    static std::shared_ptr<Shader> compile(const std::string& name);

private:
    static std::map<std::string, std::shared_ptr<Shader>> shaders;
};

#endif