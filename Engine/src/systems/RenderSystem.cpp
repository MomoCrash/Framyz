// Created by Ethan G. on 22/06/2025.

#include "RenderSystem.h"

#include "../GameManager.h"
#include "../ECS/ecs.h"

RenderSystem::RenderSystem() : Window(nullptr), DefaultPipeline(nullptr),
                               DefaultTexture(nullptr),
                               DefaultSampler(nullptr),
                               OutTexture(nullptr) {
}

RenderSystem::~RenderSystem() {
    
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

#ifdef FRAMYZ_EDITOR
    VkFormat format = RenderDevice::getInstance()->findSupportedFormat( { VK_FORMAT_B8G8R8A8_SRGB  },
        VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT );
    
    OutTexture = new RenderTarget(format, 1920, 1080);
    OutTexture->setRenderContext(&Window->getRenderContext());
#endif
    
}

void RenderSystem::update() {

    if (Window->shouldClose()) {
        GameManager::Shutdown();
        return;
    }

    Window->beginFrame();

#ifdef FRAMYZ_EDITOR
    OutTexture->beginDraw();
#else
    Window->beginDraw();
#endif
    
    Window->update(m_currentCamera);
    
    for (int i = 0; i < m_manager->getEntityCount(); i++) {
        
        Entity* entity = m_manager->getEntity(i);
        updateAsMeshRenderer(entity);
        updateAsCamera(entity);
        
    }

    DefaultPipeline->Update(Window->GetGlobalBuffer(), Window->getRenderContext().getCurrentFrame());


#ifdef FRAMYZ_EDITOR
    OutTexture->endDraw();
#else
    Window->endDraw();
    Window->display();
#endif
    
}

void RenderSystem::updateAsMeshRenderer(Entity* entity) const {
    if (!entity->hasComponent(MeshRenderer::ComponentMask)) return;
    if (MeshRenderer* meshRenderer = m_manager->getComponent<MeshRenderer>(entity)) {
            
#ifdef FRAMYZ_EDITOR
        OutTexture->drawObject(*DefaultPipeline, *meshRenderer->Object);
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
