// Created by Ethan G. on 22/06/2025.

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "BaseSystem.h"
#include "../interfaces/interfaces.h"

DECLARE_SYSTEM(RenderSystem, BaseSystem, SystemType::RENDER_SYSTEM)

    RenderSystem();
    ~RenderSystem() override;

    void preCreate() override;
    void create() override;
    void update() override;

    RenderWindow* Window;

    // Other for tests
    RenderPipeline* DefaultPipeline;
    Texture* DefaultTexture;
    Sampler* DefaultSampler;


#ifdef FRAMYZ_EDITOR
    RenderTarget* OutTexture;
#endif
    

};



#endif //RENDERSYSTEM_H
