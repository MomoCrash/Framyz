// Created by Ethan G. on 22/06/2025.

#include "EntityFactory.h"

Entity* EntityFactory::CreateEntity() {
    return GameManager::GetEntityManager().createEntity();
}

ComponentBase * EntityFactory::CreateComponent(ComponentType type) {

    //  TODO : fixme (CreateComponent)
    //  I'm not sure is this a good method to auto implement
    //  new component when I add one in ComponentType enum
    switch (static_cast<int>(type)) {
        #define VALUE(name) case static_cast<int>(ComponentType::name): return new name();
        COMPONENT_ENUM_SET
        #undef VALUE
        default: return nullptr;
    }
    
}

ComponentBase * EntityFactory::AttachComponent(ComponentType type, Entity *entityId) {

    ComponentBase* base = CreateComponent(type);
    GameManager::GetEntityManager().attachComponent(base, entityId);
    return base;
    
}
