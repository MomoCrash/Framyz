#pragma once

#include <list>

#include "../IEditorWindow.h"
#include "../../framework.h"
#include "Engine/ECS/components/ComponentBase.h"

class Entity;
class RenderObject;

class InspectorWindow : IEditorWindow
{
public:
    InspectorWindow();
    ~InspectorWindow();

    void setInspectedObject(Entity* renderObject);
    void updateParameters();

    void open() override;
    void draw() override;
    void close() override;

private:
    Entity* m_inspectedObject;
    std::vector<std::string> m_options;
};
