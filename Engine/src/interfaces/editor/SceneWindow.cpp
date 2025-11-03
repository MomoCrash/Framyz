// Created by Ethan G. on 22/06/2025.

#include "SceneWindow.h"

#include "../../systems/RenderSystem.h"

SceneWindow::SceneWindow() {}

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
}

void SceneWindow::close() {
}


void SceneWindow::draw() {
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImGui::PopStyleVar();

    ImVec2 size = ImGui::GetWindowSize();
    m_renderWindow->Window->update(size.x, size.y);
        
    ImGui::Image((ImTextureID)m_renderedImages[m_renderWindow->Window->getRenderContext().getCurrentFrame()], ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::End();
    
}
