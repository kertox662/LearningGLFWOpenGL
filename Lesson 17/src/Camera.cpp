#include "headers/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

Camera::Camera(){
    yaw = -90.0f;
    pitch = 0.0f;
    roll = 0.0f;
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    sensitivity = 0.15f;
    pitchLim = 89.0f;

    updateFront();
}

void Camera::updateView(){
    view = glm::lookAt(pos, pos+front, up);
}

void Camera::updateFront(){
    front.x = cos(glm::radians(yaw))*cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw))*cos(glm::radians(pitch));
}

void Camera::setYaw(float y){
    yaw = y;
}

void Camera::setPitch(float p){
    pitch = p;
    constrainPitch();
}

void Camera::setRoll(float r){
    roll = r;
}

void Camera::addYaw(float dy){
    yaw += dy;
}

void Camera::addPitch(float dp){
    pitch += dp;
    constrainPitch();
}

void Camera::addRoll(float dr){
    roll += dr;
}

void Camera::setSensitivity(float s){
    sensitivity = s;
}

void Camera::setPitchLimit(float pl){
    pitchLim = pl;
}

void Camera::moveCamera(std::unordered_map<int,bool> keys, float dt){
    float cameraSpeed = 2.5f * dt; // adjust accordingly
    if(keys[GLFW_KEY_W]){
        pos += cameraSpeed * front;
    }
    if(keys[GLFW_KEY_S]){
        pos -= cameraSpeed * front;
    }
    if(keys[GLFW_KEY_D]){
        pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }
    if(keys[GLFW_KEY_A]){
        pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    }
}