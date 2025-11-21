// Created by Ethan G. on 22/06/2025.

#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "../IEditorWindow.h"
#include "../../framework.h"
#include "../../ECS/components/Camera.h"

struct RenderSystem;

class SceneWindow : public IEditorWindow {

public:
    enum Layers : int{
        LAYER_SCENE,
        LAYER_PHYSICS,

        SIZE,
    };

public:
    SceneWindow();
    ~SceneWindow() = default;
    
    void setRenderWindow(RenderSystem* renderWindow);
    void addViewLayer(Layers layer, std::vector<VkImageView> const& image);

    void create() override;
    void clear() override;

    void open() override;
    void close() override;
    void draw() override;
    

private:
    
    std::map<Layers, std::vector<VkDescriptorSet>> m_renderedImages;

    RenderTarget* m_sceneOutputTexture;
    RenderSystem* m_renderWindow;
    CameraInformation m_cameraInfo;
    
    Camera* m_camera;
    float m_speed           = 10.f;
    float m_sensibility     = 50.f;

    float m_smoothScroll    = 0.f;
    float m_smoothDuration  = 0.2f;
};



#endif //SCENEWINDOW_H
