﻿#include "RenderObject.h"

RenderObject::RenderObject(Mesh* mesh)
    : m_transform(new glm::mat4())
{
    m_mesh = mesh;
}

RenderObject::~RenderObject() {}

void RenderObject::setTransform(glm::mat4* transform) {
    m_transform = transform;
}

Mesh const* RenderObject::getMesh() const
{
    return m_mesh;
}

glm::mat4 const& RenderObject::getTransform() const
{
    return *m_transform;
}