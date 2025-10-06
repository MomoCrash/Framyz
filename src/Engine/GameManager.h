// Created by Ethan G. on 22/06/2025.

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Scene.h"

#include "Entity.h"
#include "ECS/EntityManager.h"

class BaseSystem;
struct EntityFactory;

class GameManager {

public:
    GameManager();
    ~GameManager();

    static GameManager& GetInstance();

    static void Run();
    template <typename System>
    static System* AddSystem();
    // template <typename System>
    // static System* GetSystem();
    static void Update();

    static void Shutdown();

private:
    
    EntityManager m_gameManager;
    std::vector<BaseSystem*> m_systems;

    bool m_isRunning;
    
    static EntityManager& GetEntityManager();

    friend EntityFactory;
    friend Entity;
    friend BaseSystem;

};

template<typename System>
System* GameManager::AddSystem() {
    
    System* system = new System();
    GetInstance().m_systems.push_back(system);
    system->preCreate();
    return system;
    
}

// template<typename System>
// System * GameManager::GetSystem() {
//
//     for (auto m_system : GetInstance().m_systems) {
//         if (System* returnedSystem = dynamic_cast<System*>(m_system)) {
//             return returnedSystem;
//         }
//     }
//     return nullptr;
// }


#endif //GAMEMANAGER_H
