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
