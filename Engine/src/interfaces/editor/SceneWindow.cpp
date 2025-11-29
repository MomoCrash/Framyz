// Created by Ethan G. on 22/06/2025.

#include "SceneWindow.h"

#include "../../systems/RenderSystem.h"
#include <Inputs.h>

#include "../../EntityFactory.h"

SceneWindow::SceneWindow() : m_renderedImages(), m_renderWindow(nullptr), m_camera(nullptr) {
}

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
void SceneWindow::addViewLayer(SceneLayers layer, std::vector<VkImageView> const& images) {

    if (!m_renderedImages.contains(layer)) {
        m_renderedImages.emplace(layer, std::vector<VkDescriptorSet>());
    }
    
    for (VkImageView const& image : images)
        m_renderedImages[layer].push_back(ImGui_ImplVulkan_AddTexture(m_renderWindow->DefaultSampler->getSampler(), image, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL));
}

void SceneWindow::create() {
    
    GameManager::GetSystem<RenderSystem>()->CreateRenderLayer(SceneWindow::LAYER_UNLIT,     IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  &m_sceneUnlitTexture);
    GameManager::GetSystem<RenderSystem>()->CreateRenderLayer(SceneWindow::LAYER_WIREFRAME, IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  &m_sceneWireframeTexture);
    GameManager::GetSystem<RenderSystem>()->CreateRenderLayer(SceneWindow::LAYER_PHYSICS,   IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,  &m_physicsTexture);

    addViewLayer(SceneWindow::LAYER_UNLIT,      m_sceneUnlitTexture->getImages());
    addViewLayer(SceneWindow::LAYER_WIREFRAME,  m_sceneWireframeTexture->getImages());
    addViewLayer(SceneWindow::LAYER_PHYSICS,    m_physicsTexture->getImages());
    
}

void SceneWindow::clear() {
    // Parcourir toutes les layers
    for (auto& [layer, descriptorSets] : m_renderedImages) {
        // Parcourir tous les descriptors de cette layer
        for (VkDescriptorSet set : descriptorSets) {
            // Cette fonction libère le DescriptorSet créé par ImGui
            ImGui_ImplVulkan_RemoveTexture(set); 
        }
        descriptorSets.clear();
    }
    m_renderedImages.clear();
}

void SceneWindow::open() {
    Entity* entity = EntityFactory::CreateEntity();
    entity->setDebugOnly(true);
    m_camera = EntityFactory::AttachComponent<Camera>(entity);
}

void SceneWindow::close() {
}


void SceneWindow::draw() {

    float deltaTime     = GameManager::GetClock().GetDeltaTime();
    float finalSpeed    = deltaTime * m_speed;
    m_smoothScroll      = deltaTime;
    
    if (Input::GetKeyStatus(Input::W)) {
        m_camera->GetOwner()->offsetLocalPosition(-m_camera->Forward * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::S)) {
        m_camera->GetOwner()->offsetLocalPosition(m_camera->Forward * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::D)) {
        m_camera->GetOwner()->offsetLocalPosition(m_camera->Right * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::A)) {
        m_camera->GetOwner()->offsetLocalPosition(-m_camera->Right * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::SPACE)) {
        m_camera->GetOwner()->offsetLocalPosition(Transform::Up * finalSpeed);
    }
    if (Input::GetKeyStatus(Input::LEFT_SHIFT)) {
        m_camera->GetOwner()->offsetLocalPosition(-Transform::Up * finalSpeed);
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
        m_camera->Yaw += Input::GetDeltaMouseX() * deltaTime * m_sensibility;
    }
    if (Input::GetMouseButtonStatus(Input::BUTTON_RIGHT) && Input::GetDeltaMouseY() != 0) {
        m_camera->Pitch += Input::GetDeltaMouseY() * deltaTime * m_sensibility;
        m_camera->Pitch = glm::clamp(m_camera->Pitch, -89.0f, 89.0f);
    }
    
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
    ImGui::Begin("Image", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar();

    static bool hasSelected = false;
    static const char* current_item = "View 0";
    static SceneLayers selectedComponent { LAYER_UNLIT };
    
    ImGui::PushItemWidth(100.f);
    if (ImGui::BeginCombo("##view layer", current_item, ImGuiComboFlags_NoArrowButton))
    {

        for (int i = 0; i < SceneLayers::LAYER_COUNT; i++) {
            SceneLayers layer = static_cast<SceneLayers>(1 << i);
            bool is_selected = (selectedComponent == layer);
            if (ImGui::Selectable(("View mode " + std::to_string(i)).c_str(), is_selected)) {
                current_item = "View";
                selectedComponent = layer;
                m_displayedLayer = layer;
                m_renderWindow->SetCurrentActiveLayer(layer);
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    
    ImVec2 size = ImGui::GetContentRegionAvail();
    m_camera->AspectRatio = size.x / size.y;

    for (int i = 0; i < SceneLayers::LAYER_COUNT; i++) {
        SceneLayers layerType = static_cast<SceneLayers>(1 << i);
        if (!m_renderedImages.contains(layerType)) continue;
        if (!(layerType & m_displayedLayer)) continue;
        for (VkDescriptorSet imGuiDescriptor : m_renderedImages[layerType]) {
            ImGui::Image((ImTextureID)imGuiDescriptor,
            ImGui::GetWindowSize(), ImVec2(0, 0), ImVec2(1, 1));
        }
    }
    ImGui::End();
    
}
