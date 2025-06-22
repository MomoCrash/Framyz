// Created by Ethan G. on 22/06/2025.

#include "EditorSystem.h"

void EditorSystem::create() {
    
    RenderDevice::Initialize("Application");
    
    m_gui = new GuiHandler();

    m_editor = new Editor(m_gui);

    m_inspectorWindow = new InspectorWindow();
    
    renderedImages[0] = ImGui_ImplVulkan_AddTexture(m_defaultSampler->getSampler(), m_renderScene->getImage(0), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    renderedImages[1] = ImGui_ImplVulkan_AddTexture(m_defaultSampler->getSampler(), m_renderScene->getImage(1), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    m_mainWindowContext = guiHandler->inject(Window->getRenderTarget());
    m_nodeEditor = new NodeEditor(guiHandler);
    m_guiHandler = guiHandler;
    
}

void EditorSystem::update() {

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    
    ImGui_ImplVulkan_RenderDrawData(draw_data, m_renderSystem->Window->getRenderContext().getCommandBuffer());
    

}

void EditorSystem::AddRender(RenderSystem *system) {
    m_renderSystem = system;
}
