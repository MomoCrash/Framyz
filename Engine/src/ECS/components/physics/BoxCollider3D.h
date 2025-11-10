// Created by Ethan G. on 25/10/2025.

#ifndef BOXCOLLIDER3D_H
#define BOXCOLLIDER3D_H

#include "Collider3D.h"
#include "../ComponentBase.h"

DECLARE_COMPONENT( BoxCollider3D, Component ), public Collider3D
{
    BoxCollider3D();

    glm::vec3 Extend = glm::vec3(1.0f, 1.0f, 1.0f);
    
    virtual void EDITOR_Display();
    virtual void instantiate()   ;
};


#endif //BOXCOLLIDER3D_H
