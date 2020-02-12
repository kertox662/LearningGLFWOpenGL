/*
What this does:
    Draws a rotating Triangle
Technique Learned:
    Loading Shaders from files
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

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

#define GL_SILENCE_DEPRECATION 1

void processKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //If pressing Escape, Quit
        glfwSetWindowShouldClose(window, true);
    } else if(action == GLFW_PRESS){ //Otherwise on keypress
        std::cout << glfwGetKeyName(key,scancode) << std::endl;
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) //Initialize GLFW
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //The window hints start window with specific OpenGL Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Must be here for Mac OSX
    window = glfwCreateWindow(600, 600, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window); //Makes an OpenGL context

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    if(GLAD_GL_VERSION_3_3){ //Makes sure OpenGL 3.3 is supported (not really necessary with window hints)
        std::cout << "3.3 OK" << std::endl;
    }

    float tri[6] = { //Triangle Data
        0.5,0.5,
        0,  -0.5,
        -0.5,0.5
    };

    std::string vertexShader = LoadShader("res/shaders/basic.vs");
    std::string fragmentShader = LoadShader("res/shaders/basic.fs");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    unsigned int VAO; //Vertex Array Object kind of like matrix.push() and pop(), used to remember configs
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO); //Must bind (make current) so that the program knows what config to use (absolutely necessary Mac OSX)

    unsigned int bufID; //Vertex Buffer Object (VBO)
    glGenBuffers(1,&bufID);
    glBindBuffer(GL_ARRAY_BUFFER, bufID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_DYNAMIC_DRAW); //Sets the data in the buffer

    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0); //Sets what attributes are where and how big they are
    glEnableVertexAttribArray(0); //Enables certain attributes

    glUseProgram(shader); //Sets current program

    float r = 1;
    float a = 0;
    int direction = 1;

    glfwSetKeyCallback(window, processKeyPress); //GLFW key event

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        /* Render here */
        glClearColor(0.1,0.1,0.1,1); //Set clear colour
        glClear(GL_COLOR_BUFFER_BIT); //Clears viewport

        for(int i = 0; i < 6; i+=2){ //Update Tri Data
            tri[i] = r*cos(a + i* M_PI/3);
            tri[i+1] = r*sin(a + i* M_PI/3);
        }

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(tri), tri); //Update Buffer data using SubData

        glDrawArrays(GL_TRIANGLES, 0, 3); //Draws the VAO using Primitive template

        /* Swap front and back buffers */
        glfwSwapBuffers(window); //Swaps front and back buffer (They are used so no artifacts and tearing happens)

        /* Poll for and process events */
        glfwPollEvents(); //Gets events
        a += 0.02 * direction;
        if(a > M_PI_2 * 3 || a < 0) direction *= -1;
    }

    glDeleteProgram(shader);

    glfwTerminate(); //Cleans up GLFW
    return 0;
}