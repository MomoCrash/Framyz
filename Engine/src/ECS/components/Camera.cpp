// Created by Ethan G. on 18/10/2025.

#include "Camera.h"
#include "../../EntityFactory.h"

Camera::Camera() {
}

void Camera::EDITOR_Display() {
    COMPONENT_EDITOR_HEADER("Box Collider 3D");
    
    COMPONENT_EDITOR_END("Box Collider 3D")
}

void Camera::instantiate() {
    Component::instantiate();
}

void Camera::updateCamera() {
    glm::mat4 yawMat   = glm::rotate(glm::mat4(1.0f), glm::radians(Yaw),   glm::vec3(0, 1, 0));
    glm::mat4 pitchMat = glm::rotate(glm::mat4(1.0f), glm::radians(Pitch), glm::vec3(1, 0, 0));

    glm::mat4x4 rotation = yawMat * pitchMat;

    Forward = glm::normalize(glm::vec3(rotation[2]));
    Right   = glm::normalize(glm::vec3(rotation[0]));
    Up      = glm::normalize(glm::vec3(rotation[1]));
        
    Matrix = glm::translate(glm::mat4(1.0f), Owner->getLocalPosition()) * rotation;
    Matrix = glm::inverse(Matrix);
}
