#ifndef SHADER_H //SHADER_H
#define SHADER_H

#include "GLCommon.hpp"
#include <string>
#include <unordered_map>


class Shader{
private:
    unsigned int m_RendererID;
    //Uniform caching
    std::unordered_map<std::string, int> m_UniformLocations;

public:
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    const void Bind();
    const void Unbind();

    //SET UNIFORMS
    void SetUniform4f(std::string &name, float x, float y, float z, float w);

private:
    unsigned int GetUniformLocation(const std::string &name);
    std::string LoadShader(const std::string &filepath);
    unsigned int CompileShader(const unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};

#endif //SHADER_H