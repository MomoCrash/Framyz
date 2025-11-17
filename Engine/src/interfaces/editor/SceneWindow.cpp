// Created by Ethan G. on 22/06/2025.

#include "SceneWindow.h"

#include "../../systems/RenderSystem.h"
#include <Inputs.h>

#include "../../EntityFactory.h"

SceneWindow::SceneWindow() : m_renderedImages(), m_renderWindow(nullptr) {
}

SceneWindow::~SceneWindow() {}

/**
 * Set the scene render
 * @param renderWindow scene window
 */
void SceneWindow::setRenderWindow(RenderSystem *renderWindow) {
    m_renderWindow = renderWindow;
}

/**
 * Set the render image
 * @param image image index
 * @param index index in swapchain
 */
void SceneWindow::setRenderImage(VkImageView image, uint32_t index) {
    m_renderedImages[index] = ImGui_ImplVulkan_AddTexture(m_renderWindow->DefaultSampler->getSampler(), image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void SceneWindow::open() {
    Entity* entity = EntityFactory::CreateEntity();
    m_camera = EntityFactory::AttachComponent<Camera>(entity);
}

void SceneWindow::close() {
}


void SceneWindow::draw() {

    float finalSpeed = GameManager::GetClock().GetDeltaTime() * m_speed;
    m_smoothScroll += GameManager::GetClock().GetDeltaTime();
    
    if (Input::GetKeyStatus(Input::W)) {
        m_camera->GetOwner()->offsetPosition(-m_camera->GetOwner()->forward() * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::S)) {
        m_camera->GetOwner()->offsetPosition(m_camera->GetOwner()->forward() * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::D)) {
        m_camera->GetOwner()->offsetPosition(m_camera->GetOwner()->right() * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::A)) {
        m_camera->GetOwner()->offsetPosition(-m_camera->GetOwner()->right() * finalSpeed);
    }
    
    if (Input::GetMouseButtonStatus(Input::BUTTON_RIGHT) && Input::GetScrollOffsetY() > 0) {
        if (m_smoothScroll > m_smoothDuration) {
            m_speed += 20.f;
            if (m_speed > 1600.f) m_speed = 1600.f;
            m_smoothScroll = 0.f;
        }
    }
    if (Input::GetMouseButtonStatus(Input::BUTTON_RIGHT) && Input::GetScrollOffsetY() < 0) {
        if (m_smoothScroll > m_smoothDuration) {
            m_speed -= 20.f;
            if (m_speed < 1.f) m_speed = 1.f;
            m_smoothScroll = 0.f;
        }
    }
    if (Input::GetMouseButtonStatus(Input::BUTTON_RIGHT) && Input::GetDeltaMouseX() != 0) {
        m_camera->GetOwner()->rotateYPR({0, 0, Input::GetDeltaMouseX()});
    }
    if (Input::GetMouseButtonStatus(Input::BUTTON_RIGHT) && Input::GetDeltaMouseY() != 0) {
        m_camera->GetOwner()->rotateYPR({Input::GetDeltaMouseY(), 0, 0});
    } 
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PopStyleVar();
    
    ImVec2 size = ImGui::GetWindowSize();
    m_cameraInfo.Fov = m_camera->Fov;
    m_cameraInfo.AspectRatio = size.x / size.y;
    m_cameraInfo.ZFar = m_camera->ZFar;
    m_cameraInfo.ZNear = m_camera->ZNear;
    m_cameraInfo.Position = m_camera->GetOwner()->getMatrix();
    
    m_renderWindow->Window->update(m_cameraInfo);
        
    ImGui::Image((ImTextureID)m_renderedImages[m_renderWindow->Window->getRenderContext().getCurrentFrame()], ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::End();
    
}
