/*
What this does:
    Draws a Square/Image
Technique Learned:
    Textures
    How to Load a Texture
    How to Apply a Texture
*/

#include "headers/GLCommon.hpp"
#include "headers/ErrorChecking.hpp"
#include "headers/Shader.hpp"
#include "headers/SetupContext.hpp"
#include "headers/VertexBuffer.hpp"
#include "headers/IndexBuffer.hpp"
#include "headers/VertexArray.hpp"
#include "headers/Renderer.hpp"
#include "headers/Texture.hpp"

#include <cassert>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


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

    GLGenerateErrorMap();

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    Renderer renderer;

    float square[16] = { //Square Data
        0.5f,0.5f,    1.0f,1.0f,
        -0.5f,0.5f,   0.0f,1.0f,
        -0.5f,-0.5f,  0.0f,0.0f,
        0.5f,-0.5f,   1.0f, 0.0f
    };

    unsigned int indices[6] = { //Index data that represents what order to draw the vertices in
        0,1,2, //Top Tri
        2,3,0  //BottomTri
    };

    float tri[12] = { //Square Data
        0.9f,0.9f, 1.0f,1.0f,
        0.8f,0.8f, 0.0f,0.0f,
        0.9f,0.8f, 1.0f,0.0f
    };

    unsigned int triInd[3] = {
        0,1,2
    };

    
    Shader shader("res/shaders/basic.vs", "res/shaders/basic.fs");
    Texture texture("res/textures/ScotchTape.png");
    
    texture.Bind();
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    VertexArray VAO, VAO2;
    VertexBufferLayout layoutSq, layoutTri;

    layoutSq.Push<float>(2);
    layoutSq.Push<float>(2);

    layoutTri.Push<float>(2);
    layoutTri.Push<float>(2);

    VertexBuffer VBO(square, sizeof(square));
    VAO.AddBuffer(VBO, layoutSq);
    IndexBuffer IBO(indices, 6);

    VertexBuffer VBO2(tri, sizeof(tri));
    VAO2.AddBuffer(VBO2, layoutTri);
    IndexBuffer IBO2(triInd, 3);

    float col[4] = {1.0,0.0,0.0,1};

    float rate = 0.01;
    int up = 1, down = 0;
    const float tolerance = 0.0001;

    glfwSetKeyCallback(window, processKeyPress); //GLFW key event


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        renderer.Clear(0.7,0.1,0.3,1.0);

        col[up]+=rate;
        col[down]-=rate;
        if(col[down] <= tolerance){
            ++down %= 3;
            ++up   %= 3;
        }

        // shader.Bind();
        // shader.SetUniform4f("u_Color", col[0],col[1],col[2],col[3]);

        GLClearError();
        renderer.Draw(VAO, IBO, shader);
        renderer.Draw(VAO2, IBO2, shader);
        GLCheckError();

        /* Swap front and back buffers */
        glfwSwapBuffers(window); //Swaps front and back buffer (They are used so no artifacts and tearing happens)

        /* Poll for and process events */
        glfwPollEvents(); //Gets events
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}