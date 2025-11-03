// Created by Ethan G. on 22/06/2025.

#include "BaseSystem.h"

#include "../GameManager.h"

BaseSystem::BaseSystem(int mask) : m_mask(mask) {
    m_manager = &GameManager::GetInstance().GetEntityManager();
}

void BaseSystem::preCreate() {}

void BaseSystem::create() {
    Created = true;
}

void BaseSystem::fixedUpdate() {
}

void BaseSystem::destroy() {
}

int BaseSystem::getMask() {
    return m_mask;
}
