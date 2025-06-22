#pragma once

#include "framework.h"

class Mesh;

class RenderObject
{
    Mesh* m_mesh;
    glm::mat4 m_transform;

public:

    RenderObject(Mesh* mesh);
    ~RenderObject();
    
    Mesh const* getMesh() const;
    glm::mat4 const& getTransform() const;

    void setTransform(glm::mat4& transform);
    
};
