// Created by Ethan G. on 22/06/2025.

#ifndef COMPONENT_H
#define COMPONENT_H

#include "../../framework.h"

class Entity;

enum class ComponentType : uint64_t {
    
    MeshRenderer,
    
};

class ComponentBase {
    
public:
    
    ComponentBase();
    virtual ~ComponentBase() = default;
    Entity* GetOwner() const;

    void activate();
    void deactivate();

    [[nodiscard]] bool isActive() const;

    uint64_t Mask;
    Entity* Owner;
    

protected:

    bool m_active;
    
    
};

template <ComponentType typeID>
struct Component : ComponentBase
{
    Component() {
        Mask = 1 << static_cast<uint64_t>(typeID);
    }
    
    static constexpr ComponentType TypeID = typeID;
    static constexpr uint64_t ComponentMask = 1 << static_cast<uint64_t>(typeID);
};

#define DECLARE_COMPONENT( name, parent ) struct name : parent<ComponentType::name>

#endif //COMPONENT_H
