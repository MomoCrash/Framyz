// Created by Ethan G. on 25/10/2025.

#include "BoxCollider3D.h"

#include "../../../GameManager.h"
#include "../../../systems/PhysicSystem.h"
#include "../../../EntityFactory.h"


BoxCollider3D::BoxCollider3D() {
}

void BoxCollider3D::EDITOR_Display() {
    Component::EDITOR_Display();

    ImGui::SeparatorText("Box Collider Component");

    if (ImGui::Button("Remove")) {
        EntityFactory::RemoveComponent(this);
    }
    
}

void BoxCollider3D::instantiate() {
    Component::instantiate();
    
}
