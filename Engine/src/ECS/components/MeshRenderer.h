// Created by Ethan G. on 22/06/2025.

#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "ComponentBase.h"

COMPONENT_DECLARE( MeshRenderer, Component )
{
    MeshRenderer();
    
    void EDITOR_Display() override;
    void instantiate() override;

    RenderObject* Object;
};


#endif //MESHRENDERER_H
