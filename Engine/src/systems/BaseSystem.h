// Created by Ethan G. on 22/06/2025.

#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

#include "../framework.h"

class EntityManager;

namespace SystemType {
    enum SystemType : int {
        RENDER_SYSTEM           = 0x00001,
        PHYSICS_SYSTEM          = 0x00002,

#ifdef FRAMYZ_EDITOR
        EDITOR_SYSTEM           = 0xFFFFF,
#endif
    };
}

struct BaseSystem {
    explicit BaseSystem(int mask);

    virtual void preCreate();
    virtual void create();
    
    virtual void update() = 0;
    virtual void fixedUpdate();
    
    virtual void destroy();
    int getMask();

    bool Created = false;

protected:
    virtual ~BaseSystem() = default;

    int m_mask;
    EntityManager*  m_manager;
};



#endif //SYSTEMBASE_H
