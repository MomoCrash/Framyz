// Created by Ethan G. on 22/06/2025.

#include "ComponentBase.h"

#include "MeshRenderer.h"

ComponentBase::ComponentBase()
    : Mask(0), Owner(nullptr), m_active(true) {
}

Entity * ComponentBase::GetOwner() const {
    return Owner;
}

void ComponentBase::activate() {
    m_active = true;
}

void ComponentBase::deactivate() {
    m_active = false;
}

bool ComponentBase::isActive() const {
    return m_active;
}

void ComponentBase::display() {
    
}

void ComponentBase::instantiate() {
}
