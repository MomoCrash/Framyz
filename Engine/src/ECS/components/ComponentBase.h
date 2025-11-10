// Created by Ethan G. on 22/06/2025.

#ifndef COMPONENT_H
#define COMPONENT_H

#include "../../../../Tools/src/defines.h"
#include "../../framework.h"

class Entity;

#define COMPONENT_ENUM_SET   \
    VALUE(MeshRenderer)      \
    VALUE(Camera)            \
    VALUE(Rigidbody3D)       \
    VALUE(BoxCollider3D)     \
    VALUE(SphereCollider3D)  \

enum class ComponentType : uint64_t {
    #define VALUE(name) name,
    COMPONENT_ENUM_SET
    #undef VALUE
    
    Size,
};

inline std::string to_string(ComponentType c) {
    switch (c) {
        #define VALUE(name) case ComponentType::name: return #name;
        COMPONENT_ENUM_SET
        #undef VALUE
        default: return "UNKNOWN";
    }
}

class ComponentBase {
    
public:
    
    ComponentBase();
    virtual ~ComponentBase() = default;
    Entity* GetOwner() const;

    void activate();
    void deactivate();

    [[nodiscard]] bool isActive() const;

    virtual void EDITOR_Display();
    virtual void instantiate();

    bool is(uint64_t other);

    uint64_t Mask;

protected:
    Entity* Owner;
    bool m_active;

    friend class EntityManager;
    
    
};

template <ComponentType typeID>
struct Component : ComponentBase
{
    Component() {
        Mask = 1 << static_cast<uint64_t>(typeID);
    }
    
    static constexpr ComponentType TypeID   = typeID;
    static constexpr uint64_t ComponentMask = 1 << static_cast<uint64_t>(typeID);
};

#define DECLARE_COMPONENT( name, parent ) struct name : parent<ComponentType::name>

#endif //COMPONENT_H
