#include "InspectorWindow.h"

#include "defines.h"
#include "Engine/EntityFactory.h"
#include "Engine/GameManager.h"
#include "Engine/ECS/components/MeshRenderer.h"

InspectorWindow::InspectorWindow(): m_inspectedObject(nullptr) {}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::setInspectedObject(Entity* entity)
{
    m_inspectedObject = entity;
    updateParameters();
}

void InspectorWindow::updateParameters()
{
    m_options.clear();
    for (int i = 0; i < static_cast<int>(ComponentType::Size); i++) {
        m_options.push_back(to_string(static_cast<ComponentType>(i)));
    }
}

void InspectorWindow::draw()
{

    if (m_inspectedObject == nullptr) return;

    ImGui::Begin("Object Inspector");

    static bool hasSelected = false;
    static const char* current_item = "Create new Component";
    static int currentIndex = -1;
    ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;
    
    ImGuiStyle& style = ImGui::GetStyle();

    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemInnerSpacing.x;
    float button_sz = ImGui::GetFrameHeight();
    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
    if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
    {

        for (int i = 0; i < m_options.size(); i++) {
            bool is_selected = (current_item == m_options[i]);
            if (ImGui::Selectable(m_options[i].c_str(), is_selected)) {
                current_item = m_options[i].c_str();
                currentIndex = i;
                hasSelected = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (hasSelected) {
        if (ImGui::Button("Create")) {

            EntityFactory::AttachComponent(static_cast<ComponentType>(currentIndex), m_inspectedObject);
            
        }
    }

    for (auto component : m_inspectedObject->getComponents()) {
        
    }
    
    // std::string position;
    // ImGui::Text("Transform");
    // position = "X : " + std::to_string(m_inspectedObject->getPosition().x);
    // ImGui::Text(position.c_str());
    // ImGui::SameLine();
    // position = "Y : " + std::to_string(m_inspectedObject->getPosition().y);
    // ImGui::Text(position.c_str());
    // ImGui::SameLine();
    // position = "Z : " + std::to_string(m_inspectedObject->getPosition().z);
    // ImGui::Text(position.c_str());
    //
    // if (ImGui::Button("Move forward"))
    // {
    //     m_inspectedObject->offsetPosition(m_inspectedObject->forward());
    //     m_inspectedObject->update();
    // }
    // ImGui::SameLine();
    // if (ImGui::Button("Move right"))
    // {
    //     m_inspectedObject->offsetPosition(m_inspectedObject->right());
    //     m_inspectedObject->update();
    // }
    // ImGui::SameLine();
    // if (ImGui::Button("Move up"))
    // {
    //     m_inspectedObject->offsetPosition(m_inspectedObject->up());
    //     m_inspectedObject->update();
    // }
    // if (ImGui::Button("Rotate"))
    // {
    //     m_inspectedObject->rotateYPR( { 0.0f, 0.0f, 0.25f } );
    //     m_inspectedObject->update();
    // }
    
    ImGui::End();
    
}

void InspectorWindow::close() {
}

void InspectorWindow::open() {
}
