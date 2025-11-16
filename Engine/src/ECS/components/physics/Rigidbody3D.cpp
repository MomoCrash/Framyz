// Created by Ethan G. on 02/11/2025.

#include "Rigidbody3D.h"
#include "../../../EntityFactory.h"

Rigidbody3D::Rigidbody3D() {
}

void Rigidbody3D::EDITOR_Display() {
    Component::EDITOR_Display();

    COMPONENT_EDITOR_HEADER("Rigidbody 3D");
    
    COMPONENT_EDITOR_END("Rigidbody 3D")
}

void Rigidbody3D::instantiate() {
    Component::instantiate();
}
