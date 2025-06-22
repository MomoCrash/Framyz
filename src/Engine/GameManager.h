// Created by Ethan G. on 22/06/2025.

#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Scene.h"

#include "Entity.h"
#include "ECS/EntityManager.h"

class GameManager {

    EntityManager m_gameManager;

public:
    GameManager();
    ~GameManager() = default;

};



#endif //GAMEMANAGER_H
