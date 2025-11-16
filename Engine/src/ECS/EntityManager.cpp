// Created by Ethan G. on 22/06/2025.

#include "EntityManager.h"

#include "components/ComponentBase.h"
#include "../Entity.h"
#include "../GameManager.h"

EntityManager::EntityManager(): m_entityCount(0), m_entityToAddCount(0), m_entityToRemoveCount(0) {}

void EntityManager::update() {

    m_entityToRemoveCount = 0;
    for (int i = 0; i < m_entityCount; i++) {

        Entity* entity = m_entities[i]->Entity;
        entity->update();
        
        if (entity->isDestroyed()) {
            
            m_toRemoveEntityIndex[m_entityToRemoveCount++] = entity->getId();
            
        }
    }

    for (int i = 0; i < m_entityToAddCount; i++) {

        EntityComponentPair* toAddEntity = m_entitiesToAdd[i];

        m_entities[m_entityCount] = toAddEntity;
        toAddEntity->Entity->create(m_entityCount++);
        
    }
    m_entityToAddCount = 0;

    for (int i = 0; i < m_entityToRemoveCount; i++) {

        int indexRemoved = *m_toRemoveEntityIndex[i];
        EntityComponentPair* removedEntity = m_entities[indexRemoved];

        for (ComponentBase * component : removedEntity->AttachedComponents) {
            delete component;
        }
        removedEntity->AttachedComponents.clear();

        // Unique case for last entity of the array
        if (indexRemoved == m_entityCount-1) {
            delete m_entities[indexRemoved];
            m_entityCount--;
            break;
        }

        if (m_entities[indexRemoved])
            delete m_entities[indexRemoved];

        // Swap the last entity of array to deleted one
        m_entities[indexRemoved] = m_entities[m_entityCount-1];
        *m_entities[indexRemoved]->Entity->getId() = indexRemoved;
        m_entityCount--;
    }
    
    
}

void EntityManager::fixedUpdate() {
    // TODO : Implement FixedUpdate on entities
}

Entity * EntityManager::createEntity() {
    return createEntity(nullptr);
}


Entity * EntityManager::createEntity(Entity *parent) {

    Entity* newEntity = new Entity(parent);
    newEntity->preLoad(m_entityToAddCount);

    EntityComponentPair* pair = new EntityComponentPair(newEntity);
    m_entitiesToAdd[m_entityToAddCount++] = pair;
    return newEntity;
    
}

Entity * EntityManager::getEntity(int index) const {
    return m_entities[index]->Entity;
}

std::vector<ComponentBase *> const & EntityManager::getComponents(Entity* index) {
    return m_entities[*index->getId()]->AttachedComponents;
}

ComponentBase * EntityManager::getComponent(Entity *entity, uint32_t componentId) {
    if (entity->isCreated()) {
        EntityComponentPair* pair = m_entities[*entity->getId()];
        for (int i = 0; i < pair->AttachedComponents.size(); i++) {
            ComponentBase* comp = pair->AttachedComponents.at(i);
            if (comp->Mask & componentId ) {
                return comp;
            }
        }
        return nullptr;
    }
    
    EntityComponentPair* pair = m_entitiesToAdd[*entity->getId()];
    for (ComponentBase* comp : pair->AttachedComponents) {
        if (comp->Mask & componentId ) {
            return comp;
        }
    }
    return nullptr;
}

void EntityManager::attachComponent(ComponentBase *base, Entity *entity) {
    if (entity->hasComponent(base->Mask)) return;

    base->Owner = entity;
    EntityComponentPair* pair;
    
    if (entity->isCreated())
        pair = m_entities[*entity->getId()];
    else
        pair = m_entitiesToAdd[*entity->getId()];

    entity->attachComponent(base->Mask);
    pair->AttachedComponents.push_back(base);
    base->instantiate();

    for (auto system : GameManager::GetInstance().GetSystems()) {
        system->onComponentRegister(base);
    }
}

void EntityManager::removeComponent(ComponentBase *base, Entity *entity) {
    if (!entity->hasComponent(base->Mask)) return;
    
    EntityComponentPair* pair;
    
    if (entity->isCreated())
        pair = m_entities[*entity->getId()];
    else
        pair = m_entitiesToAdd[*entity->getId()];
    
    entity->removeComponent(base->Mask);
    
    for (auto system : GameManager::GetInstance().GetSystems()) {
        system->onComponentUnregister(base);
    }
    for (int i = 0; i < pair->AttachedComponents.size(); i++) {
        if (pair->AttachedComponents[i]->Mask & base->Mask) {
            base->deactivate();
            delete pair->AttachedComponents[i];
            pair->AttachedComponents.erase(pair->AttachedComponents.begin() + i);
            break;
        }
    }
}

int EntityManager::getEntityCount() const {
    return m_entityCount;
}
