#pragma once

#include "../../framework.h"
#include "../../interfaces/IEditorWindow.h"


struct NodeEditor : ImFlow::BaseNode, IEditorWindow
{
    ImFlow::ImNodeFlow  mINF;
    RenderWindow*       window;
    GuiHandler*         contextGuiHandlers;

    CameraInformation   cameraInfo;

    int index = 0;
    bool m_isOpen = false;
    
    NodeEditor(GuiHandler* handler);

    void create() override;
    void clear() override;

    void open() override;
    void close() override;
    void draw() override;
    
};
