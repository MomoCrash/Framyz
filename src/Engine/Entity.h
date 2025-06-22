// Created by Ethan G. on 22/06/2025.

#ifndef ENTITY_H
#define ENTITY_H

#include "Transform.h"

class Entity : public Transform
{
public:
    Entity();
    Entity(Entity* parent);
    ~Entity();
    
    template <typename Type>
    [[nodiscard]] Type& getComponent(uint32_t componentId);
    [[nodiscard]] bool hasComponent(uint32_t componentId) const;
    void attachComponent(uint32_t componentId);
    void removeComponent(uint32_t componentId);

    void preLoad(int id);
    void create(int id);
    void destroy();
    [[nodiscard]] int* getId();

    [[nodiscard]] bool isCreated() const;
    [[nodiscard]] bool isDestroyed() const;
    
    void SetTag(uint32_t nTag) { m_tag &= nTag; }
    [[nodiscard]] bool hasTag(uint32_t oTag) const { return m_tag & oTag; }
    [[nodiscard]] uint32_t getTag() const { return m_tag; }
    
    Transform const& getTransform() const { return m_transform; }
    

protected:
    bool m_isCreated;
    bool m_isDestoyed;
    
    int m_id;

    uint32_t m_component;
    uint32_t m_tag;
    
private:
    Transform m_transform;
};

template<typename Type>
Type & Entity::getComponent(uint32_t componentId) {
    return nullptr; // TODO Link with ECS
}

#endif //ENTITY_H
