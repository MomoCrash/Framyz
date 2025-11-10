// Created by Ethan G. on 22/06/2025.

#include "GameManager.h"

#include "systems/BaseSystem.h"
#include "Clock.h"
#include "engine.hpp"

GameManager::GameManager() : m_isRunning(false), m_mask(0), m_accumulator(0) {
}

GameManager & GameManager::GetInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::Run() {
    GameManager& manager = GetInstance();
    manager.m_isRunning = true;

    while (manager.m_isRunning) {
        manager.m_gameManager.update();

        for (auto *system: manager.m_systems) {
            if (!system->Created) system->create();
            system->update();
        }

        manager.m_accumulator += manager.m_clock.GetDeltaTime();
        float deltaTime = manager.m_clock.GetUnscaledDeltaTime();
        int steps = 0;
        while (manager.m_accumulator >= deltaTime && steps < MAX_PHYSICS_STEPS) {
            for (auto *system: manager.m_systems) {
                system->fixedUpdate();
            }
        
            manager.m_accumulator -= deltaTime;
            steps++;
        }
        if (steps == MAX_PHYSICS_STEPS) {
            manager.m_accumulator = 0.0;
        }
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
