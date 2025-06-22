#pragma once

#include "../../framework.h"
#include "InspectorWindow.h"
#include "SceneWindow.h"
#include "Engine/systems/RenderSystem.h"

class Mesh;
struct NodeEditor;

class Editor
{
public:
    Editor(GuiHandler* guiHandler);
    ~Editor();
    
    void draw();
    
};
