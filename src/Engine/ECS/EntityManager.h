// Created by Ethan G. on 22/06/2025.

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "components/ComponentBase.h"
#include "Engine/Entity.h"

class ComponentBase;
class GameManager;

class EntityManager {

    struct EntityComponentPair
    {
        Entity* Entity;
        std::vector<ComponentBase*> AttachedComponents;
    };

public:
    EntityManager();
    ~EntityManager() = default;

    void update();
    
    Entity* createEntity();
    Entity* createEntity(Entity* parent);
    [[nodiscard]] Entity* getEntity(int index) const;
    
    [[nodiscard]] std::vector<ComponentBase*> const& getComponents(Entity* entity) ;
    template<typename C>
    C* getComponent(Entity* entity) ;
    template<typename C>
    C* attachComponent(Entity* entity) ;
    void attachComponent(ComponentBase* base, Entity* entity) ;

    int getEntityCount() const;

private:
    int m_entityCount;
    int m_entityToAddCount;
    int m_entityToRemoveCount;
    
    EntityComponentPair** m_entities        = new EntityComponentPair *[2048] {nullptr};
    EntityComponentPair** m_entitiesToAdd   = new EntityComponentPair *[2048] {nullptr};
    
    int** m_toRemoveEntityIndex = new int*[2048] {nullptr};

    friend GameManager;
    friend Entity;
    
};

template<typename C>
C* EntityManager::getComponent(Entity* entity) {

    if (entity->isCreated()) {
        EntityComponentPair* pair = m_entities[*entity->getId()];
        for (int i = 0; i < pair->AttachedComponents.size(); i++) {
            ComponentBase* comp = pair->AttachedComponents.at(i);
            if (comp->Mask & C::ComponentMask ) {
                return dynamic_cast<C*>(comp);
            }
        }
        return nullptr;
    }
    
    EntityComponentPair* pair = m_entitiesToAdd[*entity->getId()];
    for (ComponentBase* comp : pair->AttachedComponents) {
        if (comp->Mask & static_cast<uint64_t>(C::TypeID) ) {
            return dynamic_cast<C*>(comp);
        }
    }
    return nullptr;
    
}

template<typename C>
C* EntityManager::attachComponent(Entity* entity)  {
    if (C* component = getComponent<C>(entity))
        return component;
    
    C* newComponent = new C();
    
    EntityManager::attachComponent(newComponent, entity);
    return newComponent;
}

#endif //ENTITYMANAGER_H
