// Created by Ethan G. on 22/06/2025.

#include "GameManager.h"

#include "systems/BaseSystem.h"
#include "Clock.h"
#include "engine.hpp"

GameManager::GameManager() : m_isRunning(false), m_mask(0), m_accumulator(0) {
}

GameManager::~GameManager() {

}

GameManager & GameManager::GetInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::Create() {
}

void GameManager::Destroy() {
    GameManager& manager = GetInstance();
    manager.m_isRunning = false;

    for (int i = 0; i < manager.m_systems.size(); i++) {
        delete manager.m_systems[i];
        manager.m_systems[i] = nullptr;
    }
    manager.m_systems.clear();
}

void GameManager::Run() {
    GameManager& manager = GetInstance();
    manager.m_isRunning = true;

    while (manager.m_isRunning) {
        manager.m_gameManager.update();

        for (auto *system: manager.m_systems) {
            if (system == nullptr) continue;
            if (!system->Created) system->create();
            system->update();
        }

        manager.m_accumulator += manager.m_clock.GetDeltaTime();
        int steps = 0;
        float deltaTime = manager.m_clock.GetFixedDeltaTime();
        while (manager.m_accumulator >= deltaTime && steps < MAX_PHYSICS_STEPS) {
            for (auto *system: manager.m_systems) {
                if (system == nullptr) continue;
                system->fixedUpdate();
            }
        
            manager.m_accumulator -= deltaTime;
            steps++;
        }
        if (steps == MAX_PHYSICS_STEPS) {
            manager.m_accumulator = 0.0;
        }
        
        manager.m_clock.UpdateTime();
    }

    for (auto* system : manager.m_systems) {
        if (system->Created) system->destroy();
    }
    
}

std::vector<BaseSystem *> const & GameManager::GetSystems() {
    return GetInstance().m_systems;
}

void GameManager::Shutdown() {
    GameManager& manager = GetInstance();
    
    manager.m_isRunning = false;
}

Clock GameManager::GetClock() {
    return GetInstance().m_clock;
}

EntityManager & GameManager::GetEntityManager() {
    return GetInstance().m_gameManager;
}
