// Created by Ethan G. on 17/10/2025.

#ifndef CAMERA_H
#define CAMERA_H

#include "ComponentBase.h"

COMPONENT_DECLARE( Camera, Component )
{
    Camera();
    
    void EDITOR_Display() override;
    void instantiate() override;

    float FOV = 0;
    
};

#endif //CAMERA_H
