// Created by Ethan G. on 22/06/2025.

#include "GameManager.h"

#include "systems/BaseSystem.h"

GameManager::GameManager(): m_gameManager() {
}

GameManager::~GameManager() {
    
}

GameManager & GameManager::GetInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::Run() {
    GameManager& manager = GetInstance();
    manager.m_isRunning = true;
    
    while (manager.m_isRunning) {
        
        for (auto* system : manager.m_systems) {
            system->update();
        }

        manager.m_gameManager.update();
        
    }
    
}

void GameManager::Update() {
    
    GetInstance().m_gameManager.update();
    
}

void GameManager::Shutdown() {
    GetInstance().m_isRunning = false;
}

EntityManager & GameManager::GetEntityManager() {
    return GetInstance().m_gameManager;
}
