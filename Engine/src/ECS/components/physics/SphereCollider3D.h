#pragma once

#include "Collider3D.h"
#include "../ComponentBase.h"

DECLARE_COMPONENT( SphereCollider3D, Component ), public Collider3D
{
    SphereCollider3D();

    float Radius = 1.0f;
    
    virtual void EDITOR_Display();
    virtual void instantiate()   ;
};