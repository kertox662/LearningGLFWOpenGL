#ifndef SETUPCONTEXT_H //SETUPCONTEXT_H
#define SETUPCONTEXT_H

#include "GLCommon.hpp"
#include <string>


GLFWwindow* MakeWindow(unsigned int width, unsigned int height, std::string title){
    GLFWwindow* window;

    if (!glfwInit()) //Initialize GLFW
        return nullptr;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //The window hints start window with specific OpenGL Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Must be here for Mac OSX
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);

    return window;
}

#endif //SETUPCONTEXT_H