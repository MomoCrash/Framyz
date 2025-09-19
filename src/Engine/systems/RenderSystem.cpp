// Created by Ethan G. on 22/06/2025.

#include "RenderSystem.h"

#include "Engine/GameManager.h"
#include "Engine/ECS/ecs.h"

RenderSystem::RenderSystem() {
    
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

    if (Window->shouldClose()) GameManager::Shutdown();

    glfwPollEvents();

    Window->beginFrame();

#ifdef FRAMYZ_EDITOR
    OutTexture->beginDraw();
#else
    Window->beginDraw();
#endif
    
    for (int i = 0; i < m_manager->getEntityCount(); i++) {
        
        Entity* entity = m_manager->getEntity(i);
        if (MeshRenderer* meshRenderer = m_manager->getComponent<MeshRenderer>(entity)) {
            entity->update();
            
#ifdef FRAMYZ_EDITOR
            OutTexture->drawObject(*DefaultPipeline, *meshRenderer->Object);
#else
            Window->drawObject(*DefaultPipeline, *meshRenderer->Object);
#endif
            
        }
    }

    DefaultPipeline->Update(Window->GetGlobalBuffer(), Window->getRenderContext().getCurrentFrame());


#ifdef FRAMYZ_EDITOR
    OutTexture->endDraw();
#else
    Window->endDraw();
    Window->display();
#endif
    
}