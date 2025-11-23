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

    bool CreateRenderLayer(SceneWindow::SceneLayers layer, ImageLayoutType layout, RenderTarget** out);
    void SetCurrentActiveLayer(SceneWindow::SceneLayers layer);

    void updateAsMeshRenderer(Entity* entity) const;
    void updateAsCamera(Entity* entity);

    std::map<SceneWindow::SceneLayers, RenderTarget*> RenderTargets;
    SceneWindow::SceneLayers Layer = SceneWindow::LAYER_UNLIT;
    RenderWindow* Window;

    // Render pipeline (different render in scene : lit, unlit, wireframed......)
    RenderPipeline* UnlitPipeline;
    RenderPipeline* WireframePipeline;

    // Default Samplers
    Texture*        DefaultTexture;
    Sampler*        DefaultSampler;

    CameraInformation m_currentCamera;

    bool m_hasCamera;
    int m_currentDisplay;


};

#endif //RENDERSYSTEM_H
