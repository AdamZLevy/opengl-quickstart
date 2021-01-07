#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


Shader::Shader(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath)
        : _rendererId(0), _vertexShaderFilePath(vertexShaderFilePath), _fragmentShaderFilePath(fragmentShaderFilePath) {
    auto vertexShader = Shader::getShader(_vertexShaderFilePath);
    auto fragmentShader = Shader::getShader(_fragmentShaderFilePath);
    this->_rendererId = Shader::createShader(vertexShader, fragmentShader);
}

Shader::~Shader() = default;

void Shader::bind() const {
    glCall(glUseProgram(this->_rendererId));
}

void Shader::unbind() const {
    glCall(glUseProgram(this->_rendererId));
}

void Shader::setUniform1i(const std::string &name, int v0) {
    glCall(glUniform1i(this->getUniformLocation(name), v0));
}

void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glCall(glUniform4f(this->getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setMatrix4fv(const std::string &name, glm::mat4 mvp) {
    glCall(glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &mvp[0][0]));
}

unsigned int Shader::getUniformLocation(const std::string &name) {
    if (_uniformLocationCache.find(name) != _uniformLocationCache.end()) {
        return _uniformLocationCache[name];
    }
    auto location = glGetUniformLocation(this->_rendererId, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;
    } else {
        _uniformLocationCache[name] = location;
    }
    return location;
}

unsigned int Shader::createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    glCall(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int Shader::compileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();

    glCall(glShaderSource(id, 1, &src, nullptr));
    glCall(glCompileShader(id));

    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << Shader::getShaderTypeStr(type) << " shader" << std::endl;
        std::cout << message << std::endl;
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

std::string Shader::getShaderTypeStr(const unsigned int type) {
    switch (type) {
        case GL_FRAGMENT_SHADER:
            return "Fragment";
        case GL_VERTEX_SHADER:
            return "Vertex";
        default:
            return std::to_string(type);
    }
}

std::string Shader::getShader(const std::string &filePath) {
    std::ifstream fileStream(filePath);
    std::stringstream shader;
    if (fileStream.is_open()) {
        shader << fileStream.rdbuf();
        fileStream.close();
    }
    return shader.str();
}
