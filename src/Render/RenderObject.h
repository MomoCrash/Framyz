#pragma once

#include "framework.h"

class Mesh;

class RenderObject
{
    Mesh* m_mesh;
    glm::mat4* m_transform;

public:

    explicit RenderObject(Mesh* mesh);
    ~RenderObject();
    
    [[nodiscard]] Mesh const* getMesh() const;
    [[nodiscard]] glm::mat4 const& getTransform() const;

    void setTransform(glm::mat4* transform);
    
};
