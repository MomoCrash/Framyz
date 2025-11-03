// Created by Ethan G. on 22/06/2025.

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Clock.h"
#include "Entity.h"
#include "ECS/EntityManager.h"
#include "systems/BaseSystem.h"

class BaseSystem;
struct EntityFactory;

class GameManager {

public:
    GameManager();
    ~GameManager() = default;

    static GameManager& GetInstance();

    static void     Run();
    
    template <typename System>
    static  System* AddSystem();

    template <typename System>
    static  System* GetSystem(int mask);
    
    static  void    Shutdown();
    
    static  Clock   GetClock();

private:
    EntityManager               m_gameManager;
    std::vector<BaseSystem*>    m_systems;
    Clock                       m_clock;

    bool                        m_isRunning;
    int                         m_mask;
    
    float                       m_accumulator;
    
    static EntityManager& GetEntityManager();

    friend EntityFactory;
    friend Entity;
    friend BaseSystem;

};

template<typename System>
System* GameManager::AddSystem() {
    
    GameManager& manager = GetInstance();
    
    System* system = new System();
    if (manager.m_mask & system->getMask()) {
        delete system;
        system = nullptr;
        return nullptr;
    }
    manager.m_systems.push_back(system);
    system->preCreate();
    manager.m_mask &= system->getMask();
    return system;
    
}

template<typename System>
System* GameManager::GetSystem(int mask) {
    GameManager& manager = GetInstance();

    for (auto system : manager.m_systems) {
        if (system->getMask() & mask) {
            return system;
        }
    }
    return nullptr;
}

#endif //GAMEMANAGER_H
