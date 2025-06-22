// Created by Ethan G. on 22/06/2025.

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "BaseSystem.h"
#include "Engine/interfaces/interfaces.h"

struct RenderSystem final : BaseSystem {

    RenderSystem();
    ~RenderSystem() override;

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
