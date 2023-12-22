#include "shader.hpp"

std::map<std::string, std::shared_ptr<Shader>> ShaderFactory::shaders;

std::shared_ptr<Shader> ShaderFactory::create(const std::string& name) {
    if (shaders.find(name) != shaders.end())
        return shaders[name];
    else {
        std::shared_ptr<Shader> shader = compile(name);
        shaders[name] = shader;
        return shaders[name];
    }
}

std::shared_ptr<Shader> ShaderFactory::compile(const std::string& name) {
    std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader());
    std::string shaderPath = FileSystem::get_path("/src/shaders/");

    // Retrieve vertex/fragment shader source code from filePath
    std::string vertexShaderCode, fragmentShaderCode;
    std::ifstream vertexShaderFile, fragmentShaderFile;
    std::stringstream vertexShaderStream, fragmentShaderStream;

    // Ensure ifstream objects can throw exceptions
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // open files
        vertexShaderFile.open(shaderPath + name + ".vert");
        fragmentShaderFile.open(shaderPath + name + ".frag");

        // read file's buffer contents into streams
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();

        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();

        // convert stream into string
        vertexShaderCode = vertexShaderStream.str();
        fragmentShaderCode = fragmentShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexShaderCode.c_str();
    const char* fShaderCode = fragmentShaderCode.c_str();

    // Compile shaders
    uint32_t vertex, fragment;
    int success;
    char infoLog[512];

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // shader program
    shader->setID(glCreateProgram());
    glAttachShader(shader->getID(), vertex);
    glAttachShader(shader->getID(), fragment);
    glLinkProgram(shader->getID());

    glGetProgramiv(shader->getID(), GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->getID(), 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shader;
}

void Shader::set(const char* uniformName, float value) const {
    use();
    glUniform1f(glGetUniformLocation(ID, uniformName), value);
}

void Shader::set(const char* uniformName, int value) const {
    use();
    glUniform1i(glGetUniformLocation(ID, uniformName), value);
}

void Shader::set(const char* uniformName, const glm::mat4& value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(ID, uniformName), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* uniformName, const glm::vec3& value) const {
    use();
    glUniform3f(glGetUniformLocation(ID, uniformName), value.x, value.y, value.z);
}

void Shader::set(const char* uniformName, float x, float y, float z) const {
    use();
    glUniform3f(glGetUniformLocation(ID, uniformName), x, y, z);
}

void Shader::use() const {
    glUseProgram(ID);
}