#pragma once

#include "Collider3D.h"
#include "../ComponentBase.h"

COMPONENT_DECLARE( SphereCollider3D, Component ), public Collider3D
{
    SphereCollider3D();

    float Radius = 1.0f;
    
    virtual void EDITOR_Display() override;
    virtual void instantiate() override;
};