/*
What this does:
    Draws 6 rotating cube with cameras
Technique Learned:
    Abstracted Camera Class
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
#include "headers/Camera.hpp"

#include <cassert>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

bool doRotate = true;

Camera cam;

std::unordered_map<int, bool> pressedKeys;


void processKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_W){
        if(action == GLFW_PRESS){
            pressedKeys[GLFW_KEY_W] = true;
        } else if(action == GLFW_RELEASE){
            pressedKeys[GLFW_KEY_W] = false;
        }
    }

    else if (key == GLFW_KEY_S){
        if(action == GLFW_PRESS){
            pressedKeys[GLFW_KEY_S] = true;
        } else if(action == GLFW_RELEASE){
            pressedKeys[GLFW_KEY_S] = false;
        }
    }

    else if (key == GLFW_KEY_A){
        if(action == GLFW_PRESS){
            pressedKeys[GLFW_KEY_A] = true;
        } else if(action == GLFW_RELEASE){
            pressedKeys[GLFW_KEY_A] = false;
        }
    }

    else if (key == GLFW_KEY_D){
        if(action == GLFW_PRESS){
            pressedKeys[GLFW_KEY_D] = true;
        } else if(action == GLFW_RELEASE){
            pressedKeys[GLFW_KEY_D] = false;
        }
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){ //If pressing Escape, Quit
        glfwSetWindowShouldClose(window, true);
    } else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        doRotate = !doRotate;
    }
}

void processMouseInput(GLFWwindow* window, double xPos, double yPos){
    static double lastX = 300, lastY = 300;

    float offsetX = xPos - lastX;
    float offsetY = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    offsetX *= cam.getSensitivity();
    offsetY *= cam.getSensitivity();

    cam.addYaw(offsetX);
    cam.addPitch(offsetY);

    cam.updateFront();
}

int main(void)
{
    float lastFrame = 0.0f, deltaTime = 0.0f, currentFrame = 0.0f;

    GLFWwindow* window = MakeWindow(600,600,"Lesson 14");
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
    
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, processKeyPress); //GLFW key event
    glfwSetCursorPosCallback(window, processMouseInput);

    glm::vec3 dirs[6];
    dirs[0] = glm::vec3(0.0f,0.0f,-3.0f);
    dirs[1] = glm::vec3(0.0f,0.0f, 3.0f);
    dirs[2] = glm::vec3(3.0f,0.0f, 0.0f);
    dirs[3] = glm::vec3(-3.0f,0.0f,0.0f);
    dirs[4] = glm::vec3(0.0f,3.0f, 0.0f);
    dirs[5] = glm::vec3(0.0f,-3.0f,0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        renderer.Clear(0.1,0.5,0.5,1.0);

        glm::mat4 mvp, proj(1.0f), view(1.0f);
        
        view = glm::lookAt(cam.getPos(), cam.getPos()+cam.getFront(), cam.getUp());
        proj = glm::perspective(glm::radians(45.0f),1.0f,0.1f,100.0f);

        for(int i = 0; i < 6; i++){
            glm::mat4 model(1.0f);
            model = glm::translate(model, dirs[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.0f,1.0f));
            mvp = proj*view*model;

            shader.SetUniformMat4fv("u_MVP", mvp);

            GLClearError();
            renderer.Draw(VAO, IBO, shader);
            GLCheckError();
        }

        glfwSwapBuffers(window);
        glfwPollEvents(); //Gets events

        angle += 1 * doRotate;
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cam.moveCamera(pressedKeys, deltaTime);
        std::cout << "(" << cam.getYaw() << ',' << cam.getPitch() << ',' << cam.getRoll() << ')' << std::endl;
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}