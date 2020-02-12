/*
What this does:
    Draws a square
Technique Learned:
    Abstraction of VAO
*/

#include "headers/GLCommon.hpp"
#include "headers/ErrorChecking.hpp"
#include "headers/ShaderLoading.hpp"
#include "headers/SetupContext.hpp"
#include "headers/VertexBuffer.hpp"
#include "headers/IndexBuffer.hpp"
#include "headers/VertexArray.hpp"

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

    float tri[6] = { //Square Data
        0.9f,0.9f,
        0.8f,0.8f,
        0.9f,0.8f
    };

    unsigned int triInd[3] = {
        0,1,2
    };

    

    std::string vertexShader = LoadShader("res/shaders/basic.vs");
    std::string fragmentShader = LoadShader("res/shaders/basic.fs");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);

    VertexArray VAO, VAO2;
    VertexBufferLayout layout;
    layout.Push<float>(2);
    // unsigned int VAO, VAO2; //Vertex Array Object kind of like matrix.push() and pop(), used to remember configs
    VertexBuffer VBO(square, sizeof(square));
    VAO.AddBuffer(VBO, layout);
    IndexBuffer IBO(indices, 6);

    VertexBuffer VBO2(tri, sizeof(tri));
    VAO2.AddBuffer(VBO2, layout);
    IndexBuffer IBO2(triInd, 3);

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

        VAO.Bind();

        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        assert(!GLCheckError());

        VAO2.Bind();

        GLClearError();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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