#include "SphereCollider3D.h"

#include "../../../EntityFactory.h"

SphereCollider3D::SphereCollider3D() {
}

void SphereCollider3D::EDITOR_Display() {
    Component<ComponentType::SphereCollider3D>::EDITOR_Display();

    ImGui::SeparatorText("Sphere Collider Component");

    if (ImGui::Button("Remove")) {
        EntityFactory::RemoveComponent(this);
    }
    
}

void SphereCollider3D::instantiate() {
    Component<ComponentType::SphereCollider3D>::instantiate();
}
