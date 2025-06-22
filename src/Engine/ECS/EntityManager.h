// Created by Ethan G. on 22/06/2025.

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "components/ComponentBase.h"
#include "Engine/Entity.h"

class ComponentBase;

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
    
    [[nodiscard]] std::vector<ComponentBase*> const& getComponents(int index) const;
    template<typename C>
    C const* getComponent(int index, ComponentType component) const;
    
    void attachComponent(ComponentBase* component, int entityIndex) const;

private:

    int m_entityCount;
    int m_entityToAddCount;
    int m_entityToRemoveCount;
    
    EntityComponentPair** m_entities        = new EntityComponentPair *[2048];
    EntityComponentPair** m_entitiesToAdd   = new EntityComponentPair *[2048];
    
    int** m_toRemoveEntityIndex = new int*[2048];
    
    friend Entity;
    
};

template<typename C>
C const* EntityManager::getComponent(int index, ComponentType component) const {
    for (ComponentBase* comp : m_entities[index]->AttachedComponents) {
        if (comp->Mask & static_cast<uint64_t>(component) ) {
            return dynamic_cast<C*>(component);
        }
    }
    return nullptr;
}

#endif //ENTITYMANAGER_H
