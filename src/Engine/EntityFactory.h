// Created by Ethan G. on 22/06/2025.

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "GameManager.h"
#include "ECS/EntityManager.h"
#include "ECS/components/MeshRenderer.h"

struct EntityFactory {

    static Entity* CreateEntity();
    template <typename C>
    static C* AttachComponent(Entity* entityId);

    static ComponentBase* CreateComponent(ComponentType type);
    static ComponentBase* AttachComponent(ComponentType type, Entity* entityId);

};

template<typename C>
C* EntityFactory::AttachComponent(Entity* entity) {
    return GameManager::GetEntityManager().attachComponent<C>(entity);
}


#endif //ENTITYFACTORY_H
