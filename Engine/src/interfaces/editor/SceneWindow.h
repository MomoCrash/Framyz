// Created by Ethan G. on 22/06/2025.

#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "../IEditorWindow.h"
#include "../../framework.h"
#include "../../ECS/components/Camera.h"

struct RenderSystem;

class SceneWindow : public IEditorWindow {

public:
    enum SceneLayers : uint32_t {
        
        LAYER_UNLIT     = 0x0001,
        LAYER_WIREFRAME = 0x0002,
        LAYER_PHYSICS   = 0x0004,

        LAYER_COUNT     = 3,
    };

public:
    SceneWindow();
    ~SceneWindow() override = default;
    
    void setRenderWindow(RenderSystem* renderWindow);
    void addViewLayer(SceneLayers layer, std::vector<VkImageView> const& image);

    void create() override;
    void clear() override;

    void open() override;
    void close() override;
    void draw() override;

private:
    
    std::unordered_map<SceneLayers, std::vector<VkDescriptorSet>> m_renderedImages;

    RenderTarget* m_sceneUnlitTexture;
    RenderTarget* m_sceneWireframeTexture;
    RenderTarget* m_physicsTexture;
    
    RenderSystem* m_renderWindow;
    CameraInformation m_cameraInfo;
    
    Camera* m_camera;
    float   m_speed           = 10.f;
    float   m_sensibility     = 50.f;

    float   m_smoothScroll    = 0.f;
    float   m_smoothDuration  = 0.2f;

    SceneLayers  m_displayedLayer  = LAYER_UNLIT;
};



#endif //SCENEWINDOW_H
