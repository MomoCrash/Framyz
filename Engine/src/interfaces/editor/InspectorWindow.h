#pragma once

#include <list>

#include "../IEditorWindow.h"
#include "../../framework.h"
#include "../../ECS/components/ComponentBase.h"

class Entity;
class RenderObject;

class InspectorWindow : IEditorWindow
{

    struct OptionsComponent
    {
        int Id;
        std::string Name;
    };
    
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
    std::vector<OptionsComponent> m_options;

    static inline float* m_position[3] = {
        0,
        0,
        0
    };
    static inline float* m_rotation[3] = {
        0,
        0,
        0
    };
};
