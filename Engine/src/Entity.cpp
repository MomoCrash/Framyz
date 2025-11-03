// Created by Ethan G. on 22/06/2025.

#include "Entity.h"

#include "GameManager.h"

Entity::Entity()
    : m_component(0), m_tag(0), m_isCreated(false), m_isDestoyed(false), m_id(0) {}

Entity::Entity(Entity *parent)
    : m_component(0), m_tag(0), m_isCreated(false), m_isDestoyed(false), m_id(0) {}

Entity::~Entity() {}

ComponentBase * Entity::getComponent(uint32_t componentId) {
    //return GameManager::GetEntityManager().getComponent(this);
    return nullptr;
}

std::vector<ComponentBase *> const& Entity::getComponents() {
    return GameManager::GetEntityManager().getComponents(this);
    
}

/// Check if the entity has the current component attach to it
/// @param componentId mask of the component
/// @return if has the component
bool Entity::hasComponent(uint32_t componentId) const {
    return (m_component & componentId);
}

/// Set component attached to the entity
/// @param componentId mask of the component 
void Entity::attachComponent(uint32_t componentId) {
    m_component |= componentId;
}

/// Remove attachment of the component
/// @param componentId mask of the component 
void Entity::removeComponent(uint32_t componentId) {
    m_component ^= componentId;
}

void Entity::preLoad(int id) {
    m_id = id;
}

void Entity::create(int id) {
    m_isCreated = true;
    m_id = id;
}

int* Entity::getId() {
    return &m_id;
}

bool Entity::isCreated() const {
    return m_isCreated;
}

bool Entity::isDestroyed() const {
    return m_isDestoyed;
}

void Entity::destroy() {
    m_isDestoyed = true;
}
