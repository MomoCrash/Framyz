// Created by Ethan G. on 25/10/2025.

#ifndef BOXCOLLIDER3D_H
#define BOXCOLLIDER3D_H

#include "Collider3D.h"
#include "../ComponentBase.h"

COMPONENT_DECLARE( BoxCollider3D, Component ), public Collider3D
{
    BoxCollider3D();

    glm::vec3 Extend = glm::vec3(0.5f, 0.5f, 0.5f);

    void EDITOR_Display() override;
    void instantiate() override;
};


#endif //BOXCOLLIDER3D_H
