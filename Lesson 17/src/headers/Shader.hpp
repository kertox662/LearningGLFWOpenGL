#ifndef SHADER_H //SHADER_H
#define SHADER_H

#include "GLCommon.hpp"
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>


class Shader{
private:
    unsigned int m_RendererID;
    //Uniform caching
    std::unordered_map<std::string, int> m_UniformLocations;

public:
    Shader(const std::string &vertexFile, const std::string &fragmentFile);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    //SET UNIFORMS
    void SetUniform1i(std::string name, int value);
    void SetUniform3f(std::string name, float x, float y, float z);
    void SetUniform4f(std::string name, float x, float y, float z, float w);
    void SetUniformMat3fv(std::string name, glm::mat3 mat);
    void SetUniformMat4fv(std::string name, glm::mat4 mat);

private:
    unsigned int GetUniformLocation(const std::string &name);
    std::string LoadShader(const std::string &filepath);
    unsigned int CompileShader(const unsigned int type, const std::string &source);
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
};

#endif //SHADER_H