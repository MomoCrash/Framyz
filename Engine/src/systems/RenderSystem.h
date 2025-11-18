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

    void updateAsMeshRenderer(Entity* entity) const;
    void updateAsCamera(Entity* entity);

    RenderWindow* Window;

    // Other for tests
    RenderPipeline* DefaultPipeline;
    Texture* DefaultTexture;
    Sampler* DefaultSampler;

    CameraInformation m_currentCamera;

    bool m_hasCamera;
    int m_currentDisplay;
#ifdef FRAMYZ_EDITOR
    RenderTarget* OutTexture;
#endif

};



#endif //RENDERSYSTEM_H
