// Created by Ethan G. on 22/06/2025.

#ifndef ENTITY_H
#define ENTITY_H

class ComponentBase;
class EntityManager;
#include "Transform.h"

class Entity : public Transform
{
public:
    Entity();
    Entity(Entity* parent);
    ~Entity();
    
    [[nodiscard]] ComponentBase* getComponent(uint64_t componentId);
    [[nodiscard]] std::vector<ComponentBase*> const& getComponents();
    [[nodiscard]] bool hasComponent(uint64_t componentId) const;

    void preLoad(int id);
    void create(int id);
    
    void destroy() { m_isDestroyed = true; };
    [[nodiscard]] int* getId() { return &m_id; };
    
    [[nodiscard]] bool isCreated() const { return m_isCreated; };
    [[nodiscard]] bool isDestroyed() const { return m_isDestroyed; };

    [[nodiscard]] bool isEnable() const { return m_isEnable; };
    [[nodiscard]] bool isDebugOnly() const { return m_isDebugOnly; };
    
    void setEnable(bool enable) { m_isEnable = enable; };
    void setDebugOnly(bool debugOnly) { m_isDebugOnly = debugOnly; };
    
    void SetTag(uint32_t nTag) { m_tag &= nTag; }
    [[nodiscard]] bool hasTag(uint32_t oTag) const { return m_tag & oTag; }
    [[nodiscard]] uint32_t getTag() const { return m_tag; }
    
    Transform& getTransform() { return m_transform; }

protected:
    bool m_isCreated;
    bool m_isDestroyed;
    bool m_isEnable;
    bool m_isDebugOnly;
    
    int m_id;

    uint32_t m_component;
    uint32_t m_tag;

    void attachComponent(uint64_t componentId);
    void removeComponent(uint64_t componentId);
    
private:
    Transform m_transform;

    friend EntityManager;
};

#endif //ENTITY_H
