#pragma once

#include "../IEditorWindow.h"
#include "../../framework.h"

class Entity;
class RenderObject;

class InspectorWindow : IEditorWindow
{
public:
    InspectorWindow();
    ~InspectorWindow();


    void setInspectedObject(Entity* renderObject);

    void open() override;
    void draw() override;
    void close() override;

private:
    Entity* m_inspectedObject;
};
