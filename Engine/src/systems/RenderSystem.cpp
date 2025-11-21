// Created by Ethan G. on 22/06/2025.

#include "RenderSystem.h"

#include "../GameManager.h"
#include "../ECS/ecs.h"

RenderSystem::RenderSystem() : Window(nullptr), DefaultPipeline(nullptr),
                               DefaultTexture(nullptr),
                               DefaultSampler(nullptr) {
}

RenderSystem::~RenderSystem() {
    
    for (auto& [Layer, Targets] : RenderTargets)
        delete Targets;
    RenderTargets.clear();
    
    delete Window;

    delete DefaultPipeline;
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
    
    DefaultPipeline = new RenderPipeline(*DefaultTexture, *DefaultSampler, Window->getRenderTarget(), { &sFragment, &sVertex});
    
}

void RenderSystem::update() {

    if (Window->shouldClose()) {
        Window->flushCommand();
        vkDeviceWaitIdle(RenderDevice::getInstance()->getDevice());
        GameManager::Destroy();
        return;
    }

    Window->beginFrame();

    for (auto& [Layer, Targets] : RenderTargets)
        Targets->beginDraw();
    
    Window->update(m_currentCamera);
    DefaultPipeline->Update(Window->GetGlobalBuffer(), Window->getRenderContext().getCurrentFrame());
    
    for (int i = 0; i < m_manager->getEntityCount(); i++) {
        
        Entity* entity = m_manager->getEntity(i);
        updateAsMeshRenderer(entity);
        updateAsCamera(entity);
        
    }

    /////////////////////////////////////////////////////////////////////////////////////
    ////////A FINIR T4ES EN TRAIN DE FAIRE LE SYSTEM DE MULTI RENDU               /////////
    ////////A FINIR T4ES EN TRAIN DE FAIRE LE SYSTEM DE MULTI RENDU               /////////
    ////////A FINIR T4ES EN TRAIN DE FAIRE LE SYSTEM DE MULTI RENDU               /////////
    ////////A FINIR T4ES EN TRAIN DE FAIRE LE SYSTEM DE MULTI RENDU               /////////
    ////////A FINIR T4ES EN TRAIN DE FAIRE LE SYSTEM DE MULTI RENDU               /////////
    /////////////////////////////////////////////////////////////////////////////////////

    for (auto& [Layer, Targets] : RenderTargets)
        Targets->endDraw();

    Window->beginDraw();
    for (auto& system : GameManager::GetSystems())
        if (system->Created) system->draw();
    Window->endDraw();
    
    Window->display();
}

bool RenderSystem::CreateRenderLayer(SceneWindow::Layers layer, ImageLayoutType layout, RenderTarget** out) {
    if (RenderTargets.contains(layer)) return false;
    
    VkFormat format = RenderDevice::getInstance()->findSupportedFormat( { VK_FORMAT_B8G8R8A8_SRGB  },
VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT );
    
    *out = new RenderTarget(format, 1920, 1080, layout);
    (*out)->setRenderContext(&Window->getRenderContext());

    RenderTargets.emplace(layer, (*out));
    return true;
}

void RenderSystem::updateAsMeshRenderer(Entity* entity) const {
    if (!entity->hasComponent(MeshRenderer::ComponentMask)) return;
    if (MeshRenderer* meshRenderer = m_manager->getComponent<MeshRenderer>(entity)) {
            
#ifdef FRAMYZ_EDITOR
        RenderTargets.at(SceneWindow::Layers::LAYER_SCENE)->drawObject(*DefaultPipeline, *meshRenderer->Object);
#else
        Window->drawObject(*DefaultPipeline, *meshRenderer->Object);
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
