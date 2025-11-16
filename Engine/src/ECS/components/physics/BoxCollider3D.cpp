// Created by Ethan G. on 25/10/2025.

#include "BoxCollider3D.h"

#include "../../../GameManager.h"
#include "../../../systems/PhysicSystem.h"
#include "../../../EntityFactory.h"


BoxCollider3D::BoxCollider3D() {
}

void BoxCollider3D::EDITOR_Display() {
    COMPONENT_EDITOR_HEADER("Box Collider 3D");
    
    COMPONENT_EDITOR_END("Box Collider 3D")
}

void BoxCollider3D::instantiate() {
    Component::instantiate();
    
}
