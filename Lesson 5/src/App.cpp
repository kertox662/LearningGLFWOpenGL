/*
What this does:
    Draws a square
Technique Learned:
    Uniforms
*/

#include "headers/GLCommon.hpp"
#include "headers/ErrorChecking.hpp"
#include "headers/ShaderLoading.hpp"
#include "headers/SetupContext.hpp"

#include <cassert>
#include <cmath>


void processKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //If pressing Escape, Quit
        glfwSetWindowShouldClose(window, true);
    } else if(action == GLFW_PRESS){ //Otherwise on keypress
        std::cout << glfwGetKeyName(key,scancode) << std::endl;
    }
}

int main(void)
{
    GLFWwindow* window = MakeWindow(600,600,"Lesson 5");
    if(!window)
        return -1;

    //V-Sync
    // glfwSwapInterval(1);

    GLGenerateErrorMap();

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    float square[8] = { //Square Data
        0.5f,0.5f,
        -0.5f,0.5f,
        -0.5f,-0.5f,
        0.5f,-0.5f
    };

    unsigned int indices[6] = { //Index data that represents what order to draw the vertices in
        0,1,2, //Top Tri
        2,3,0  //BottomTri
    };

    std::string vertexShader = LoadShader("res/shaders/basic.vs");
    std::string fragmentShader = LoadShader("res/shaders/basic.fs");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    unsigned int VAO; //Vertex Array Object kind of like matrix.push() and pop(), used to remember configs
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO); //Must bind (make current) so that the program knows what config to use (absolutely necessary Mac OSX)

    unsigned int VBO; //Vertex Buffer Object (VBO)
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_DYNAMIC_DRAW); //Sets the data in the buffer

    glVertexAttribPointer(0,2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0); //Sets what attributes are where and how big they are
    glEnableVertexAttribArray(0); //Enables certain attributes

    unsigned int IBO; //Index Buffer Object (IBO)
    glGenBuffers(1,&IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); //GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW); //Sets the data in the IBO

    glUseProgram(shader); //Sets current program

    float col[4] = {1.0,0.0,0.0,1};

    int location = glGetUniformLocation(shader, "u_Color");
    assert(location != -1);

    float rate = 0.01;
    int up = 1, down = 0;
    const float tolerance = 0.0001;

    glfwSetKeyCallback(window, processKeyPress); //GLFW key event

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        /* Render here */
        glClearColor(0.1,0.1,0.1,1); //Set clear colour
        glClear(GL_COLOR_BUFFER_BIT); //Clears viewport

        col[up]+=rate;
        col[down]-=rate;
        if(col[down] <= tolerance){
            ++down %= 3;
            ++up   %= 3;
        }

        glUniform4f(location, col[0],col[1],col[2],col[3]);

        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); //Draws using an IBO
        //If IBO is not bound, then "nullptr" should be a pointer to IBO
        assert(!GLCheckError());

        /* Swap front and back buffers */
        glfwSwapBuffers(window); //Swaps front and back buffer (They are used so no artifacts and tearing happens)

        /* Poll for and process events */
        glfwPollEvents(); //Gets events
    }

    glDeleteProgram(shader);

    glfwTerminate(); //Cleans up GLFW
    return 0;
}