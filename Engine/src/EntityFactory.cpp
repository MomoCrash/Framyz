// Created by Ethan G. on 22/06/2025.

#include "EntityFactory.h"

Entity* EntityFactory::CreateEntity() {
    return GameManager::GetEntityManager().createEntity();
}

ComponentBase * EntityFactory::CreateComponent(ComponentType type) {

    return EntityFactory::GetInstance()->ComponentBuilder[static_cast<int>(type)]();
    
}

ComponentBase * EntityFactory::AttachComponent(ComponentType type, Entity *entityId) {

    ComponentBase* base = CreateComponent(type);
    GameManager::GetEntityManager().attachComponent(base, entityId);
    return base;
    
}

void EntityFactory::RemoveComponent(ComponentBase *component) {
    GameManager::GetEntityManager().removeComponent(component, component->GetOwner());
}
