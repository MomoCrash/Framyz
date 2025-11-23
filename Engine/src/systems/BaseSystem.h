// Created by Ethan G. on 22/06/2025.

#ifndef SYSTEMBASE_H
#define SYSTEMBASE_H

#include "../framework.h"
#include "../interfaces/editor/SceneWindow.h"

class ComponentBase;
class EntityManager;

namespace SystemType {
    enum Type : int {
        RENDER_SYSTEM           = 0x00001,
        PHYSICS_SYSTEM          = 0x00002,

#ifdef FRAMYZ_EDITOR
        EDITOR_SYSTEM           = 0x00010,
#endif
    };
}

struct BaseSystem {
    explicit BaseSystem();
    virtual ~BaseSystem() = default;

    virtual void preCreate();
    virtual void create();
    
    virtual void update()   = 0;
    virtual void drawRenderTarget(SceneWindow::SceneLayers layer, RenderTarget* target);
    virtual void drawWindow();
    virtual void fixedUpdate();
    
    virtual void destroy();

    virtual void onComponentRegister(ComponentBase* component);
    virtual void onComponentUnregister(ComponentBase* component);

    virtual int getMask() = 0;
    bool        Created = false;

protected:
    EntityManager*  m_manager;
};

#define DECLARE_SYSTEM( name, parent, type ) \
struct name : parent {  \
    static constexpr int Mask = type;\
    int getMask() override { \
        return name::Mask;\
    }\
    


#endif //SYSTEMBASE_H
