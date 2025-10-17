// Created by Ethan G. on 22/06/2025.

#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include "GameManager.h"
#include "ECS/EntityManager.h"
#include "ECS/components/MeshRenderer.h"
#include "ECS/components/Camera.h"

struct EntityFactory {
private:
    /// Utils function to create a component on initialization
    /// @tparam Type 
    /// @return 
    template<typename Type>
    static ComponentBase* CreateComponent() {
        return new Type();
    }
    
    const std::function<ComponentBase* ()>* ComponentBuilder = new std::function<ComponentBase * ()>[static_cast<int>(ComponentType::Size)] {
        #define VALUE(name) EntityFactory::CreateComponent<name>,
        COMPONENT_ENUM_SET
        #undef VALUE
    };

    static EntityFactory* GetInstance() {
        static EntityFactory factory;
        return &factory;
    }

public:
    static Entity* CreateEntity();
    template <typename C>
    static C* AttachComponent(Entity* entityId);

    static ComponentBase* CreateComponent(ComponentType type);
    static ComponentBase* AttachComponent(ComponentType type, Entity* entityId);
    static void RemoveComponent(ComponentBase* component);

};

template<typename C>
C* EntityFactory::AttachComponent(Entity* entity) {
    return GameManager::GetEntityManager().attachComponent<C>(entity);
}


#endif //ENTITYFACTORY_H
