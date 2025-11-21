// Created by Ethan G. on 22/06/2025.

#ifndef EDITORSYSTEM_H
#define EDITORSYSTEM_H

#include "BaseSystem.h"
#include "RenderSystem.h"
#include "../interfaces/interfaces.h"

class HierarchyWindow;

DECLARE_SYSTEM(EditorSystem, BaseSystem, SystemType::EDITOR_SYSTEM)
    EditorSystem() : m_guiIndex(0){}
    ~EditorSystem() override;

    void create() override;
    void update() override;
    void draw() override;

    void AddRender(RenderSystem* system);
    void AttachWindow(IEditorWindow* window);
    template<class EditorWindow>
    EditorWindow* getWindowByType();

    //==== Externals windows ====//
    NodeEditor*                     getNodeEditor();

private:

    GuiHandler*                     m_gui               = nullptr;

    std::vector<IEditorWindow*>     m_editorWindows;
     
    RenderSystem*                   m_renderSystem      = nullptr;
    NodeEditor*                     m_nodeEditor        = nullptr;

    RenderTarget*                   m_editorRender      = nullptr;

int m_guiIndex;
    
};

template<class EditorWindow>
EditorWindow * EditorSystem::getWindowByType() {
    for (auto& attachedWindows : m_editorWindows) {
        if (EditorWindow* window = reinterpret_cast<EditorWindow*>(attachedWindows)) {
            return window;
        }
    }
    return nullptr;
}

#endif //EDITORSYSTEM_H
