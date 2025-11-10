// Created by Ethan G. on 22/06/2025.

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Clock.h"
#include "Entity.h"
#include "ECS/EntityManager.h"
#include "systems/BaseSystem.h"

class HierarchyWindow;
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
    static  System* GetSystem();
    
    static  std::vector<BaseSystem*> const& GetSystems();
    
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

#ifdef FRAMYZ_EDITOR
    friend HierarchyWindow;
#endif
    
    friend EntityFactory;
    friend Entity;
    friend BaseSystem;

};

template<typename System>
System* GameManager::AddSystem() {
    
    GameManager& manager = GetInstance();
    
    System* system = new System();
    int mask = system->getMask();
    if (manager.m_mask & mask) {
        delete system;
        system = nullptr;
        return nullptr;
    }
    manager.m_systems.push_back(system);
    system->preCreate();
    manager.m_mask |= system->getMask();
    return system;
    
}

template<typename System>
System* GameManager::GetSystem() {
    GameManager& manager = GetInstance();

    for (auto system : manager.m_systems) {
        if (system->getMask() & System::Mask) {
            return dynamic_cast<System *>(system);
        }
    }
    return nullptr;
}

#endif //GAMEMANAGER_H
