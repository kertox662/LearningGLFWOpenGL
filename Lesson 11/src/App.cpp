/*
What this does:
    Draws scotch tape rotated in 3d space
Technique Learned:
    glm matrices
    rotation
    translation
    projection
    mvp matrix
    matrix uniform for shaders
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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void processKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //If pressing Escape, Quit
        glfwSetWindowShouldClose(window, true);
    } else if(key < 256 && action == GLFW_PRESS){ //Otherwise on keypress
        std::cout << glfwGetKeyName(key,scancode) << std::endl;
    }
}

int main(void)
{
    GLFWwindow* window = MakeWindow(600,600,"Lesson 12");
    if(!window)
        return -1;

    GLGenerateErrorMap();

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    Renderer renderer;

    float square[20] = { //Square Data
        0.5f,0.5f,   0.0f,    1.0f,1.0f,
        -0.5f,0.5f,  0.0f,    0.0f,1.0f,
        -0.5f,-0.5f, 0.0f,    0.0f,0.0f,
        0.5f,-0.5f,  0.0f,    1.0f, 0.0f
    };

    unsigned int indices[6] = { //Index data that represents what order to draw the vertices in
        0,1,2, //Top Tri
        2,3,0  //BottomTri
    };

    float tri[15] = { //Triangle Data
        0.9f,0.9f,0.0f, 1.0f,1.0f,
        0.8f,0.8f,0.0f, 0.0f,0.0f,
        0.9f,0.8f,0.0f, 1.0f,0.0f
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

    layoutSq.Push<float>(3);
    layoutSq.Push<float>(2);

    layoutTri.Push<float>(3);
    layoutTri.Push<float>(2);

    VertexBuffer VBO(square, sizeof(square));
    VAO.AddBuffer(VBO, layoutSq);
    IndexBuffer IBO(indices, 6);

    VertexBuffer VBO2(tri, sizeof(tri));
    VAO2.AddBuffer(VBO2, layoutTri);
    IndexBuffer IBO2(triInd, 3);

    float angle = 0;

    glfwSetKeyCallback(window, processKeyPress); //GLFW key event
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        renderer.Clear(0.1,0.1,0.1,1.0);

        glm::mat4 mvp, model(1.0f), view(1.0f), proj(1.0f);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.0f,0.0f));
        view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        proj = glm::perspective(glm::radians(45.0f),1.0f,0.1f,100.0f);
        // proj = glm::ortho(-2.0f,2.0f,-2.0f,2.0f,0.1f,100.0f);
        mvp = proj*view*model;

        shader.SetUniformMat4fv("u_MVP", mvp);

        GLClearError();
        renderer.Draw(VAO, IBO, shader);
        renderer.Draw(VAO2, IBO2, shader);
        GLCheckError();

        glfwSwapBuffers(window);
        glfwPollEvents(); //Gets events
        angle += 1;
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}