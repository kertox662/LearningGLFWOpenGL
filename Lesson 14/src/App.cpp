/*
What this does:
    Draws 6 rotating cube with cameras
Technique Learned:
    Camera-like functions
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

double baseRad = 10;

float yaw = -90.0f, pitch = 0, roll = 0;
const double rad = 3;
float camX = 0;
float camY = 0;
float camZ = 0;
glm::vec3 pos(camX, camY, camZ), dir(0.0f, 0.0f, -1.0f);

float lastFrame = 0.0f, deltaTime = 0.0f;

std::unordered_map<int, bool> pressedKeys;

void makeVector(const float yaw, const float pitch, const float roll, glm::vec3& direction){
    direction.x = cos(yaw)*cos(pitch);
    direction.y = sin(pitch);
    direction.z = sin(yaw)*cos(pitch);
}

void moveCamera(std::unordered_map<int, bool>& keys){
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime; // adjust accordingly

    if(keys[GLFW_KEY_W]){
        pos += cameraSpeed * dir;
    }
    if(keys[GLFW_KEY_S]){
        pos -= cameraSpeed * dir;
    }
    if(keys[GLFW_KEY_D]){
        pos += glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    }
    if(keys[GLFW_KEY_A]){
        pos -= glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraSpeed;
    }
}

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
    const float sensitivity = 0.05f;
    const float pitchLim = 89.0f;

    static double lastX = 300, lastY = 300;

    float offsetX = xPos - lastX;
    float offsetY = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    offsetX *= sensitivity;
    offsetY *= sensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if(pitch > pitchLim) pitch = pitchLim;
    else if(pitch < -pitchLim) pitch = -pitchLim;

    makeVector(glm::radians(yaw), glm::radians(pitch), glm::radians(roll), dir);
}

int main(void)
{
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
        renderer.Clear(0.1,0.1,0.1,1.0);

        glm::mat4 mvp, proj(1.0f), view(1.0f);
        
        view = glm::lookAt(pos, pos+dir, glm::vec3(0.0f,1.0f,0.0f));
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
        moveCamera(pressedKeys);
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}