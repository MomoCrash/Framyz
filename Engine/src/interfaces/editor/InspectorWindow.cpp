#include "InspectorWindow.h"

#include "../../EntityFactory.h"
#include "../../GameManager.h"
#include "../../ECS/components/MeshRenderer.h"

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
        if (!m_inspectedObject->hasComponent(1 << i)) {
            OptionsComponent option {i, to_string(static_cast<ComponentType>(i))};
            m_options.push_back(option);
        }
    }

    m_position[0] = &m_inspectedObject->getLocalPosition().x;
    m_position[1] = &m_inspectedObject->getLocalPosition().y;
    m_position[2] = &m_inspectedObject->getLocalPosition().z;
    
    m_rotation[0] = &m_inspectedObject->getLocalRotation().x;
    m_rotation[1] = &m_inspectedObject->getLocalRotation().y;
    m_rotation[2] = &m_inspectedObject->getLocalRotation().z;
}

void InspectorWindow::draw()
{

    if (m_inspectedObject == nullptr) return;
    if (!m_inspectedObject->isCreated()) return;

    ImGui::Begin("Object Inspector");

    static bool hasSelected = false;
    static const char* current_item = "Create new Component";
    static OptionsComponent selectedComponent {-1};
    ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;
    
    ImGuiStyle& style = ImGui::GetStyle();

    float w = ImGui::CalcItemWidth();
    float spacing = style.ItemInnerSpacing.x;
    float button_sz = ImGui::GetFrameHeight();
    
    ImGui::PushItemWidth(w - spacing * 2.0f - button_sz * 2.0f);
    if (ImGui::BeginCombo("##custom combo", current_item, ImGuiComboFlags_NoArrowButton))
    {

        for (int i = 0; i < m_options.size(); i++) {
            bool is_selected = (current_item == m_options[i].Name);
            if (ImGui::Selectable(m_options[i].Name.c_str(), is_selected)) {
                current_item = m_options[i].Name.c_str();
                selectedComponent.Id = m_options[i].Id;
                hasSelected = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (hasSelected) {
        ImGui::SameLine(0, 10);
        if (ImGui::Button("Create")) {

            EntityFactory::AttachComponent(static_cast<ComponentType>(selectedComponent.Id), m_inspectedObject);
            hasSelected = false;
            current_item = "Create new Component";
            updateParameters();
        }
    }

    ImGui::SeparatorText("Transform");
    if (ImGui::DragFloat3("Position", *m_position, 0.1)) {
        m_inspectedObject->setDirty();
    }
    if (ImGui::DragFloat3("Rotation", *m_rotation, 0.1)) {
        m_inspectedObject->setDirty();
        m_inspectedObject->updateRotation();
    }

    
    for (auto component : m_inspectedObject->getComponents()) {
        component->EDITOR_Display();
    }
    
    ImGui::End();
    
}

void InspectorWindow::close() {
}

void InspectorWindow::open() {
}
