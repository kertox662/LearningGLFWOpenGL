#ifndef SHADERLOADING_H //SHADERLOADING_H
#define SHADERLOADING_H

#include "GLCommon.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

static std::string LoadShader(const std::string &filepath){ //Get Shaders from files
    std::ifstream shaderStream(filepath);
    std::stringstream shader;
    std::string line;
    
    while(getline(shaderStream, line)){
        shader << line;
        shader << std::endl;
    }
    return shader.str();
}

static unsigned int CompileShader(const unsigned int type, const std::string &source){ //Compiles a shader and returns its ID
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

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader){
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

#endif //SHADERLOADING_H