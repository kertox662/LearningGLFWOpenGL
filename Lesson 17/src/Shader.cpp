#include "headers/Shader.hpp"
#include "headers/ErrorChecking.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertexFile, const std::string &fragmentFile){
    std::string vSource = LoadShader(vertexFile), fSource = LoadShader(fragmentFile);
    m_RendererID = CreateShader(vSource, fSource);
}

Shader::~Shader(){
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const{
    glUseProgram(m_RendererID);
}
void Shader::Unbind() const{
    glUseProgram(0);
}

void Shader::SetUniform1i(std::string name, int value){
    int location = GetUniformLocation(name);
    if(location != -1){
        glUniform1i(location, value);
    }
}

void Shader::SetUniform3f(std::string name, float x, float y, float z){
    int location = GetUniformLocation(name);
    if(location != -1)
        glUniform3f(location,x,y,z);
}

void Shader::SetUniform4f(std::string name, float x, float y, float z, float w){
    int location = GetUniformLocation(name);
    if(location != -1)
        glUniform4f(location, x,y,z,w);
}

void Shader::SetUniformMat3fv(std::string name, glm::mat3 mat){
    int location = GetUniformLocation(name);
    if(location != -1)
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetUniformMat4fv(std::string name, glm::mat4 mat){
    int location = GetUniformLocation(name);
    if(location != -1)
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

unsigned int Shader::GetUniformLocation(const std::string &name){
    
    if(m_UniformLocations.find(name) != m_UniformLocations.end()){
        return m_UniformLocations[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if(location == -1){
        std::cout << "The uniform " << name << " does not exist for this shader." << std::endl;
    }
    m_UniformLocations[name] = location;
    return location;
}

std::string Shader::LoadShader(const std::string &filepath){
    std::ifstream shaderStream(filepath);
    std::stringstream shader;
    std::string line;
    
    while(getline(shaderStream, line)){
        shader << line;
        shader << std::endl;
    }
    return shader.str();
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type); //Makes empty shader
    const char* src = source.c_str(); 
    glShaderSource(id,1,&src,nullptr); //Adds data to shader (Must be C-String)
    glCompileShader(id); //Compiles

    //Reminder - Do Error Handling for the Shader source code.
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); //Gets compilation status
    if(result == GL_FALSE){ //If failed compiling
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); //Gets how long the error message is
        char message[length];
        glGetShaderInfoLog(id, length, &length, message); //Puts data into message buffer
        std::cout << "FAILED " << ((type == GL_VERTEX_SHADER)?"vertex":"fragment") << " Shader Compilation!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); //Deletes shader object
        return 0;
    }

    return id; //Gives back the id for referencing the shader program
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader){
    unsigned int program = glCreateProgram(); //The full program

    unsigned int vs = CompileShader(GL_VERTEX_SHADER,vertexShader); //Vertex and Fragment Shaders
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs); //Attaching basically makes them as a single object
    glAttachShader(program, fs);
    glLinkProgram(program); //Linking combines them
    glValidateProgram(program); //Checks linking errors

    glDeleteShader(vs); //Can Delete compiled shader objects now
    glDeleteShader(fs);

    return program; //Returns program ID
}
