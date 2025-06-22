// Created by Ethan G. on 22/06/2025.

#ifndef MESHRENDERER_H
#define MESHRENDERER_H
#include "ComponentBase.h"


DECLARE_COMPONENT( MeshRenderer, Component )
{
    MeshRenderer();

    RenderObject* Object;
};


#endif //MESHRENDERER_H
