// Created by Ethan G. on 17/10/2025.

#ifndef CAMERA_H
#define CAMERA_H

#include "ComponentBase.h"

COMPONENT_DECLARE( Camera, Component )
{
    Camera();
    
    void EDITOR_Display() override;
    void instantiate() override;

    float   Fov             = 75.0f;
    float   AspectRatio     = 4.f/3.f;
    float   ZNear           = 0.1f;
    float   ZFar            = 256.f;
        
    float   Yaw             = 0.0f;  
    float   Pitch           = 0.0f;
    int     Display         = 0;

    glm::vec3 Forward       = glm::vec3(1.0f, 0.0f, 0.0f); 
    glm::vec3 Right         = glm::vec3(0.0f, 1.0f, 0.0f); 
    glm::vec3 Up            = glm::vec3(0.0f, 0.0f, 1.0f);
    
    glm::mat4x4 Matrix      = glm::mat4x4(1.0f); 

    void updateCamera();
    
};

#endif //CAMERA_H
