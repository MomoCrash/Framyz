// Created by Ethan G. on 22/06/2025.

#include "SceneWindow.h"

SceneWindow::SceneWindow() {}

SceneWindow::~SceneWindow() {}

void SceneWindow::draw(RenderWindow* window) {
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ImVec2 size = ImGui::GetWindowSize();
    m_renderWindow->update(size.x, size.y);
        
    ImGui::Image((ImTextureID)renderedImages[m_renderWindow->getRenderContext().getCurrentFrame()], ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::End();
    
}
