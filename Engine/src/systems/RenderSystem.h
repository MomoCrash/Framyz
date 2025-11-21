// Created by Ethan G. on 22/06/2025.

#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "BaseSystem.h"
#include "../interfaces/interfaces.h"


DECLARE_SYSTEM(RenderSystem, BaseSystem, SystemType::RENDER_SYSTEM)

public:
    RenderSystem();
    ~RenderSystem() override;

    void preCreate() override;
    void create() override;

    void update() override;

    bool CreateRenderLayer(SceneWindow::Layers layer, ImageLayoutType layout, RenderTarget** out);
    void updateAsMeshRenderer(Entity* entity) const;
    void updateAsCamera(Entity* entity);

    std::map<SceneWindow::Layers, RenderTarget*> RenderTargets;
    RenderWindow* Window;

    // Other for tests
    RenderPipeline* DefaultPipeline;
    Texture*        DefaultTexture;
    Sampler*        DefaultSampler;

    CameraInformation m_currentCamera;

    bool m_hasCamera;
    int m_currentDisplay;


};

#endif //RENDERSYSTEM_H
