// Created by Ethan G. on 22/06/2025.

#include "EntityFactory.h"

Entity* EntityFactory::CreateEntity() {
    return GameManager::GetEntityManager().createEntity();
}
