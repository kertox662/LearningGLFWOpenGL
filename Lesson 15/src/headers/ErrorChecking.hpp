#ifndef ERROR_CHECKCKING_H //ERROR_CHECKCKING_H
#define ERROR_CHECKCKING_H

#include "GLCommon.hpp"
#include <unordered_map>
#include <iostream>

static std::unordered_map<unsigned int,std::string> errorCodes; //A map from the Error Codes from glGetError to a string representation

static void GLGenerateErrorMap(){ //Populates errorCodes
    errorCodes[GL_INVALID_ENUM]                  = "Invalid Enum";
    errorCodes[GL_INVALID_VALUE]                 = "Invalid Value";
    errorCodes[GL_INVALID_OPERATION]             = "Invalid Operation";
    errorCodes[GL_INVALID_FRAMEBUFFER_OPERATION] = "Invalid FrameBuffer Operation";
    errorCodes[GL_OUT_OF_MEMORY]                 = "Out of Memory";
}

static void GLClearError(){ //Gets rid of all of the errors from glGetError
    while(glGetError());
}

static bool GLCheckError(){ //Prints out all of the errors from glGetError
    bool gotError = false;
    while(GLenum err = glGetError()){
        gotError = true;
        std::cout << "|OpenGL Error| (" << errorCodes[err] << ")" << std::endl;
    }
    return gotError;
}

#endif //ERROR_CHECKCKING_H