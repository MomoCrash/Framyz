// Created by Ethan G. on 22/06/2025.

#include "RenderSystem.h"

#include <ranges>

#include "../GameManager.h"
#include "../ECS/ecs.h"

RenderSystem::RenderSystem() : Window(nullptr), UnlitPipeline(nullptr),
                               DefaultTexture(nullptr),
                               DefaultSampler(nullptr) {
}

RenderSystem::~RenderSystem() {
    
    for (auto& [Layer, Targets] : RenderTargets)
        delete Targets;
    RenderTargets.clear();
    
    delete Window;

    delete UnlitPipeline;
    delete DefaultSampler;
    delete DefaultTexture;
    
}

void RenderSystem::preCreate() {
    
    BaseSystem::preCreate();

    RenderDevice::Initialize("Framyz Application");

    Window = new RenderWindow("Framyz Editor", 700, 700);
    
}

void RenderSystem::create() {

    BaseSystem::create();

    Shader sFragment("shader.frag", Shader::FRAGMENT);
    Shader sVertex("shader.vert", Shader::VERTEX);
    
    DefaultTexture = new Texture(Window->getRenderTarget(), "sunflower.jpg");
    DefaultSampler = new Sampler();
    
    UnlitPipeline = new RenderPipeline(*DefaultTexture, *DefaultSampler, Window->getRenderTarget(), POLYGON_MODE_FILL, { &sFragment, &sVertex});
    WireframePipeline = new RenderPipeline(*DefaultTexture, *DefaultSampler, Window->getRenderTarget(), POLYGON_MODE_LINE, { &sFragment, &sVertex});
    
}

void RenderSystem::update() {

    if (Window->shouldClose()) {
        Window->flushCommand();
        vkDeviceWaitIdle(RenderDevice::getInstance()->getDevice());
        GameManager::Destroy();
        return;
    }

    Window->beginFrame();

    for (auto[LayerType, Targets] : RenderTargets) {
        if (Layer & LayerType && Targets != nullptr)
            Targets->beginDraw();
    }
    
    Window->update(m_currentCamera);
    
    UnlitPipeline->Update(Window->GetGlobalBuffer(), Window->getRenderContext().getCurrentFrame());
    WireframePipeline->Update(Window->GetGlobalBuffer(), Window->getRenderContext().getCurrentFrame());
    
    for (int i = 0; i < m_manager->getEntityCount(); i++) {
        
        Entity* entity = m_manager->getEntity(i);
        updateAsMeshRenderer(entity);
        updateAsCamera(entity);
        
    }

    for (auto& system : GameManager::GetSystems())
        if (system->Created) system->drawRenderTarget(Layer, RenderTargets[Layer]);

    for (auto[LayerType, Targets] : RenderTargets) {
        if (Layer & LayerType && Targets != nullptr)
            Targets->endDraw();
    }

    Window->beginDraw();
    for (auto& system : GameManager::GetSystems())
        if (system->Created) system->drawWindow();
    Window->endDraw();
    
    Window->display();
}

bool RenderSystem::CreateRenderLayer(SceneWindow::SceneLayers layer, ImageLayoutType layout, RenderTarget** out) {
    if (RenderTargets.contains(layer)) {
        if (RenderTargets[layer] != nullptr) {
            *out = RenderTargets[layer];
            return false;
        }
    }
    
    VkFormat format = RenderDevice::getInstance()->findSupportedFormat( { VK_FORMAT_B8G8R8A8_SRGB  },
VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT );

    RenderTargetInformation infos{};
    infos.format = format;
    infos.width = 1920;
    infos.height = 1080;
    infos.type = layout;
    infos.useMSAA = true;
    *out = new RenderTarget(&Window->getRenderContext(), infos);

    RenderTargets[layer] = *out;
    return true;
}

void RenderSystem::SetCurrentActiveLayer(SceneWindow::SceneLayers layer) {
    Layer = layer;
}

void RenderSystem::updateAsMeshRenderer(Entity* entity) const {
    if (!entity->hasComponent(MeshRenderer::ComponentMask)) return;
    if (MeshRenderer* meshRenderer = m_manager->getComponent<MeshRenderer>(entity)) {

        if (meshRenderer->Object == nullptr) return;
            
#ifdef FRAMYZ_EDITOR

        if (Layer & SceneWindow::SceneLayers::LAYER_UNLIT && RenderTargets.contains(SceneWindow::SceneLayers::LAYER_UNLIT))
            RenderTargets.at(SceneWindow::SceneLayers::LAYER_UNLIT)->drawObject(*UnlitPipeline, *meshRenderer->Object);
        
        if (Layer & SceneWindow::SceneLayers::LAYER_WIREFRAME && RenderTargets.contains(SceneWindow::SceneLayers::LAYER_WIREFRAME))
            RenderTargets.at(SceneWindow::SceneLayers::LAYER_WIREFRAME)->drawObject(*WireframePipeline, *meshRenderer->Object);

        #else
        Window->drawObject(*UnlitPipeline, *meshRenderer->Object);
#endif
            
    }
}

void RenderSystem::updateAsCamera(Entity *entity) {
    if (!entity->hasComponent(Camera::ComponentMask)) return;
    if (Camera* camera = m_manager->getComponent<Camera>(entity)) {

        camera->updateCamera();
        
        m_currentCamera.Fov     = camera->Fov;
        m_currentCamera.AspectRatio = camera->AspectRatio;
        m_currentCamera.ZFar    = camera->ZFar;
        m_currentCamera.ZNear   = camera->ZNear;
        m_currentCamera.View    = camera->Matrix;
    
        Window->update(m_currentCamera);
            
    }
}
