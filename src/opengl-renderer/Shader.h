//
// Created by Adam on 11/15/2020.
//

#ifndef OPENGL_QUICKSTART_SHADER_H
#define OPENGL_QUICKSTART_SHADER_H


#include <string>
#include <unordered_map>

class Shader {
private:
    unsigned int _type;
    std::string _filePath;
    unsigned int _rendererId;
    std::unordered_map<std::string, int> _uniformLocationCache;
public:
    explicit Shader(unsigned int type, const std::string &filePath);

    ~Shader();

    void bind() const;

    void unbind() const;

    void setUniform1i(const std::string& name, int v0);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    unsigned int getUniformLocation(const std::string& name);
    static unsigned int compileShader(unsigned int type, const std::string &source);
    static unsigned int createShader(unsigned int type, const std::string &shader);
    static std::string getShaderTypeStr(unsigned int type);
    static std::string getShader(const std::string &filePath);
};


#endif //OPENGL_QUICKSTART_SHADER_H
