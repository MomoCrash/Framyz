// Created by Ethan G. on 22/06/2025.

#include "Entity.h"

#include "GameManager.h"

Entity::Entity()
    : m_component(0), m_tag(0), m_isCreated(false), m_isDestroyed(false), m_isEnable(false), m_isDebugOnly(false),
      m_id(0) {
}

Entity::Entity(Entity *parent)
    : m_component(0), m_tag(0), m_isCreated(false), m_isDestroyed(false), m_isEnable(false), m_isDebugOnly(false),
      m_id(0) {
}

Entity::~Entity() {}

ComponentBase * Entity::getComponent(uint64_t componentId) {
    return GameManager::GetEntityManager().getComponent(this, componentId);
    return nullptr;
}

std::vector<ComponentBase *> const& Entity::getComponents() {
    return GameManager::GetEntityManager().getComponents(this);
    
}

/// Check if the entity has the current component attach to it
/// @param componentId mask of the component
/// @return if has the component
bool Entity::hasComponent(uint64_t componentId) const {
    return (m_component & componentId);
}

/// Set component attached to the entity
/// @param componentId mask of the component 
void Entity::attachComponent(uint64_t componentId) {
    m_component |= componentId;
}

/// Remove attachment of the component
/// @param componentId mask of the component 
void Entity::removeComponent(uint64_t componentId) {
    m_component ^= componentId;
}

void Entity::preLoad(int id) {
    m_id = id;
}

void Entity::create(int id) {
    m_isCreated = true;
    m_id = id;
}