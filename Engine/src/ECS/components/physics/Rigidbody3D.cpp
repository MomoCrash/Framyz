// Created by Ethan G. on 02/11/2025.

#include "Rigidbody3D.h"

#include "../../../EntityFactory.h"

Rigidbody3D::Rigidbody3D() {
}

void Rigidbody3D::EDITOR_Display() {
    Component::EDITOR_Display();

    ImGui::SeparatorText("Rigidbody3D Component");

    if (ImGui::Button("Remove")) {
        EntityFactory::RemoveComponent(this);
    }
}

void Rigidbody3D::instantiate() {
    Component::instantiate();
}
