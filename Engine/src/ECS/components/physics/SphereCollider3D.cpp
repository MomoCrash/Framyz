#include "SphereCollider3D.h"
#include "../../../EntityFactory.h"

SphereCollider3D::SphereCollider3D() {
}

void SphereCollider3D::EDITOR_Display() {
    Component<ComponentType::SphereCollider3D>::EDITOR_Display();

    COMPONENT_EDITOR_HEADER("Sphere Collider 3D");
    
    COMPONENT_EDITOR_END("Sphere Collider 3D")
    
}

void SphereCollider3D::instantiate() {
    Component<ComponentType::SphereCollider3D>::instantiate();
}
