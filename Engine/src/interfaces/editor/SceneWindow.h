// Created by Ethan G. on 22/06/2025.

#ifndef SCENEWINDOW_H
#define SCENEWINDOW_H

#include "../IEditorWindow.h"
#include "../../framework.h"
#include "../../ECS/components/Camera.h"

struct RenderSystem;

class SceneWindow : IEditorWindow {

public:
    SceneWindow();
    ~SceneWindow();
    
    void setRenderWindow(RenderSystem* renderWindow);
    void setRenderImage(VkImageView image, uint32_t index);

    void open() override;
    void close() override;
    void draw() override;
    

private:
    VkDescriptorSet m_renderedImages[RenderWindow::MAX_FRAMES_IN_FLIGHT];
    
    RenderSystem* m_renderWindow;
    CameraInformation m_cameraInfo;
    
    Camera* m_camera;
    float m_speed           = 10.f;
    float m_sensibility     = 50.f;

    float m_smoothScroll    = 0.f;
    float m_smoothDuration  = 0.2f;
};



#endif //SCENEWINDOW_H
