// Created by Ethan G. on 22/06/2025.

#ifndef EDITORSYSTEM_H
#define EDITORSYSTEM_H

#include "BaseSystem.h"
#include "RenderSystem.h"
#include "../interfaces/interfaces.h"

struct EditorSystem final : BaseSystem {
    
    void create() override;
    void update() override;

    void AddRender(RenderSystem* system);

private:

    GuiHandler*      m_gui             = nullptr;
    
    InspectorWindow* m_inspectorWindow = nullptr;
    SceneWindow*     m_sceneWindow     = nullptr;
    NodeEditor*      m_nodeEditor      = nullptr;
     
    RenderSystem*    m_renderSystem    = nullptr;

    int m_guiIndex;
    
};


#endif //EDITORSYSTEM_H
