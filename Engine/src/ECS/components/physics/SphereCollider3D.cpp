#include "SphereCollider3D.h"

SphereCollider3D::SphereCollider3D() {
}

void SphereCollider3D::EDITOR_Display() {
    Component<ComponentType::SphereCollider3D>::EDITOR_Display();
}

void SphereCollider3D::instantiate() {
    Component<ComponentType::SphereCollider3D>::instantiate();
}
