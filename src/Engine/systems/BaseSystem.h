// Created by Ethan G. on 22/06/2025.

#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

#include "Engine/ECS/EntityManager.h"

struct BaseSystem {

    virtual void create() = 0;
    virtual void update() = 0;

protected:
    BaseSystem();
    virtual ~BaseSystem() = default;

    EntityManager* m_manager;
};



#endif //SYSTEMBASE_H
