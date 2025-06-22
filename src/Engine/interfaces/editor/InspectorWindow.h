#pragma once

#include "../../framework.h"

class Entity;
class RenderObject;

class InspectorWindow
{
public:
    InspectorWindow();
    ~InspectorWindow();

    void draw();
    void setInspectedObject(Entity* renderObject);

private:
    Entity* m_inspectedObject;
};
