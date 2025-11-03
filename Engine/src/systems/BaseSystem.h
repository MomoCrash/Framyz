// Created by Ethan G. on 22/06/2025.

#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

#include "../framework.h"

class EntityManager;

struct BaseSystem {

    virtual void preCreate();
    virtual void create();
    virtual void update() = 0;
    virtual void fixedUpdate();

    bool Created = false;

protected:
    BaseSystem();
    virtual ~BaseSystem() = default;
    
    EntityManager* m_manager;
};



#endif //SYSTEMBASE_H
