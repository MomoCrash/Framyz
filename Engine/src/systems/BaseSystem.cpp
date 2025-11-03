// Created by Ethan G. on 22/06/2025.

#include "BaseSystem.h"

#include "../GameManager.h"

void BaseSystem::preCreate() {}

void BaseSystem::create() {
    Created = true;
}

void BaseSystem::fixedUpdate() {
}

BaseSystem::BaseSystem() {
    m_manager = &GameManager::GetEntityManager();
}
