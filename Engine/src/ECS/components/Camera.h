// Created by Ethan G. on 17/10/2025.

#ifndef CAMERA_H
#define CAMERA_H

#include "ComponentBase.h"

COMPONENT_DECLARE( Camera, Component )
{
    Camera();
    
    void EDITOR_Display() override;
    void instantiate() override;

    float Fov = 75.0f;
    float AspectRatio = 4.f/3.f;
    float ZNear = 0.1f;
    float ZFar = 256.f;
    
};

#endif //CAMERA_H
