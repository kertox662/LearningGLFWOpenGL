/*
What this does:
    Light Abstraction
Technique Learned:
    Materials struct
    Light struct
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
#include <glm/gtc/matrix_inverse.hpp>

bool doRotate = true;

Camera cam;

std::unordered_map<int, bool> pressedKeys;

struct Material{
    glm::vec3 ambient, diffuse,specular;
    float shininess;
};

struct Light{
    glm::vec3 pos, ambient, diffuse, specular;
};

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

    GLFWwindow* window = MakeWindow(600,600,"Lesson 16");
    if(!window)
        return -1;

    GLGenerateErrorMap();

    int gladErr = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); //Starts up GLAD
    assert(gladErr != 0); //Checks glad is ok

    Renderer renderer;

    #include "cube.h"
    #include "data.h"

    
    Shader shader("res/shaders/basic.vs", "res/shaders/basic.fs");
    Shader lightingShader("res/shaders/basic.vs", "res/shaders/lighting.fs");
    Texture texture("res/textures/crate2.png");
    
    texture.Bind();
    shader.Bind();
    // shader.SetUniform1i("u_Texture", 0);

    VertexArray VAO, lightVAO;
    VertexBufferLayout layoutSq;

    layoutSq.Push<float>(3);
    layoutSq.Push<float>(3);


    VertexBuffer VBO(cube, sizeof(cube)), lightVBO(cube, sizeof(cube));
    VAO.AddBuffer(VBO, layoutSq);
    lightVAO.AddBuffer(lightVBO, layoutSq);
    IndexBuffer IBO(indices, sizeof(indices) / sizeof(unsigned int));
    IndexBuffer lightIBO(indices, sizeof(indices) / sizeof(unsigned int));

    float angle = 0;
    
    // glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, processKeyPress); //GLFW key event
    glfwSetCursorPosCallback(window, processMouseInput);

    

    glm::mat4 mvp, proj(1.0f);
    proj = glm::perspective(glm::radians(45.0f),1.0f,0.1f,100.0f);

    shader.Bind();
    shader.SetUniformMat4fv("u_Proj", proj);

    lightingShader.Bind();
    lightingShader.SetUniformMat4fv("u_Proj", proj);
    // lightingShader.SetUniform3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
    // lightingShader.SetUniform3f("u_LightColor",  1.0f, 1.0f, 1.0f);

    float baseRad = 5.0f;

    glm::vec3 ambient(0.1f,0.1f,0.1f), diffuse(0.7f,0.7f,0.7f), specular(0.5f,0.5f,0.5f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) //Render Loop while not closing window
    {
        renderer.Clear(0.1,0.1,0.1,1.0);

        glm::mat4 lightModel(1.0f);
        glm::vec3 lightPos(baseRad*cos(glfwGetTime()/2), 2*sin(2*glfwGetTime()), baseRad*sin(glfwGetTime()/2));
        lightModel = glm::translate(lightModel, lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2));

        Light light = {lightPos, ambient, diffuse, specular};

        glm::mat4 view(1.0f);
        
        view = glm::lookAt(cam.getPos(), cam.getPos()+cam.getFront(), cam.getUp());

        glm::vec3 camPos = cam.getPos();

        shader.Bind();

        lightingShader.Bind();
        lightingShader.SetUniformMat4fv("u_View",view);
        lightingShader.SetUniform3f("u_ViewPos",camPos.x, camPos.y, camPos.z);
        lightingShader.SetUniform3f("u_Light.pos",light.pos);
        lightingShader.SetUniform3f("u_Light.ambient",light.ambient);
        lightingShader.SetUniform3f("u_Light.diffuse",light.diffuse);
        lightingShader.SetUniform3f("u_Light.specular",light.specular);
        
        for(int i = 0; i < 6; i++){
            glm::mat4 model(1.0f);
            glm::mat3 normalMatrix, model3;
            model = glm::translate(model, dirs[i]);
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f,0.0f,1.0f));

            model3 = glm::mat3(model);
            normalMatrix = glm::inverseTranspose(model3);

            lightingShader.SetUniformMat4fv("u_Model",model);
            lightingShader.SetUniformMat3fv("u_NormalMatrix",normalMatrix);
            // lightingShader.SetUniform3f("u_LightPos",lightPos.x, lightPos.y, lightPos.z);

            lightingShader.SetUniform3f("u_Mat.ambient", mats[i].ambient);
            lightingShader.SetUniform3f("u_Mat.diffuse", mats[i].diffuse);
            lightingShader.SetUniform3f("u_Mat.specular", mats[i].specular);
            lightingShader.SetUniform1f("u_Mat.shininess", mats[i].shininess);

            renderer.Draw(VAO, IBO, lightingShader);
        }

        shader.Bind();
        shader.SetUniformMat4fv("u_Model", lightModel);
        shader.SetUniformMat4fv("u_View", view);

        renderer.Draw(lightVAO, lightIBO, shader);

        glfwSwapBuffers(window);
        glfwPollEvents(); //Gets events

        angle += 1 * doRotate;

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        cam.moveCamera(pressedKeys, deltaTime);
    }

    glfwTerminate(); //Cleans up GLFW
    return 0;
}