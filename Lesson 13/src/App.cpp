/*
What this does:
    Draws rotating cube controlled by the mouse position
Technique Learned:
    glm::lookat
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

bool doRotate = true;

void processKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //If pressing Escape, Quit
        glfwSetWindowShouldClose(window, true);
    } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        doRotate = !doRotate;
    }
}

double baseRad = 10;

double curAngle = 0, rad = 3;
float camX = rad;
float camY = 0;
float camZ = 0;

void processMouseInput(GLFWwindow* window, double xpos, double ypos){
    if(xpos < 0 || xpos > 600 || ypos < 1 || ypos > 600) return;
    curAngle = xpos/300 * M_PI;

    double pitchA = (600.0 - ypos) / 1200.0 * M_PI;
    camY = sin(pitchA) * baseRad;
    rad = cos(pitchA) * baseRad;

    camY = sqrt(600.0f - (float)ypos);
    // rad = camY;

    camX = cos(curAngle) * rad;
    camZ = sin(curAngle) * rad;

    std::cout << "(" << camX << ',' << camY << ',' << camZ << ')' << ' ' << rad << std::endl;

    // std::cout << glm::degrees(curAngle) << std::endl;
}

int main(void)
{
    GLFWwindow* window = MakeWindow(600,600,"Lesson 13");
    if(!window)
        return -1;

    GLGenerateErrorMap();

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    Renderer renderer;

    float cube[70] = { //Square Data
        -0.5f, 0.5f, -0.5f,   0.0f, 1.0f,
        0.5f, 0.5f, -0.5f,   0.3334f, 1.0f,
        0.5f, 0.5f, 0.5f,   0.6667f, 1.0f,
        -0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.75f,
        0.5f, -0.5f, -0.5f,   0.3334f, 0.75f,
        0.5f, -0.5f, 0.5f,   0.6667f, 0.75f,
        -0.5f, -0.5f, 0.5f,   1.0f, 0.75f,
        -0.5f, -0.5f, -0.5f,   0.3334f, 0.5f,
        -0.5f, -0.5f, 0.5f,   0.6667f, 0.5f,
        -0.5f, 0.5f, -0.5f,   0.3334f, 0.25f,
        -0.5f, 0.5f, 0.5f,   0.6667f, 0.25f,
        0.5f, 0.5f, -0.5f,   0.3334f, 0.0f,
        0.5f, 0.5f, 0.5f,   0.6667f, 0.0f
    };

    unsigned int indices[36] = { //Index data that represents what order to draw the vertices in
        0,1,4,
        1,4,5,
        1,2,5,
        2,5,6,
        2,3,6,
        3,6,7,
        5,6,8,
        6,8,9,
        8,9,10,
        9,10,11,
        10,11,12,
        11,12,13
    };

    
    Shader shader("res/shaders/basic.vs", "res/shaders/basic.fs");
    Texture texture("res/textures/crate2.png");
    
    texture.Bind();
    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    VertexArray VAO, VAO2;
    VertexBufferLayout layoutSq;

    layoutSq.Push<float>(3);
    layoutSq.Push<float>(2);


    VertexBuffer VBO(cube, sizeof(cube));
    VAO.AddBuffer(VBO, layoutSq);
    IndexBuffer IBO(indices, sizeof(indices) / sizeof(unsigned int));

    float angle = 0;

    glfwSetKeyCallback(window, processKeyPress); //GLFW key event
    glfwSetCursorPosCallback(window, processMouseInput);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        renderer.Clear(0.1,0.1,0.1,1.0);

        glm::mat4 mvp, model(1.0f), view(1.0f), proj(1.0f);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.0f,1.0f));
        // view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
        view = glm::lookAt(glm::vec3(camX,camY,camZ), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
        proj = glm::perspective(glm::radians(45.0f),1.0f,0.1f,100.0f);
        // proj = glm::perspective(glm::radians( (float) ((int)angle%360) ),1.0f,0.1f,100.0f);
        // proj = glm::perspective(glm::radians(45.0f),1.0f,0.1f,fabs(cos(glm::radians(angle/10)))*5);
        mvp = proj*view*model;

        shader.SetUniformMat4fv("u_MVP", mvp);

        GLClearError();
        renderer.Draw(VAO, IBO, shader);
        GLCheckError();

        glfwSwapBuffers(window);
        glfwPollEvents(); //Gets events

        angle += 1 * doRotate;
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}