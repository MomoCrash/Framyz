// Created by Ethan G. on 22/06/2025.

#include "BaseSystem.h"

#include "../GameManager.h"


BaseSystem::BaseSystem() {
    m_manager = &GameManager::GetInstance().GetEntityManager();
}

void BaseSystem::preCreate() {}

void BaseSystem::create() {
    Created = true;
}

void BaseSystem::drawRenderTarget(SceneWindow::SceneLayers layer, RenderTarget *target) {
}

void BaseSystem::drawWindow() {}

void BaseSystem::fixedUpdate() {}

void BaseSystem::destroy() {}

void BaseSystem::onComponentRegister(ComponentBase *component) {}

void BaseSystem::onComponentUnregister(ComponentBase *component) {}
