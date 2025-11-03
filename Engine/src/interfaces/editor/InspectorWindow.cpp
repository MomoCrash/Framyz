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
        if (!m_inspectedObject->hasComponent(i))
            m_options.push_back(to_string(static_cast<ComponentType>(i)));
    }
}

void InspectorWindow::draw()
{

    if (m_inspectedObject == nullptr) return;
    if (!m_inspectedObject->isCreated()) return;

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
        ImGui::SameLine(0, 10);
        if (ImGui::Button("Create")) {

            EntityFactory::AttachComponent(static_cast<ComponentType>(currentIndex), m_inspectedObject);
            hasSelected = false;
            updateParameters();
        }
    }

    ImGui::SeparatorText("Transform");
    static float* position[3] = { &m_inspectedObject->getPosition().x, &m_inspectedObject->getPosition().y, &m_inspectedObject->getPosition().z};
    ImGui::DragFloat3("Position", *position, 0.1);
    static float* rotation[3] = { &m_inspectedObject->getRotation().x, &m_inspectedObject->getRotation().y, &m_inspectedObject->getRotation().z};
    glm::vec3 oldRotation = glm::vec3(m_inspectedObject->getRotation());
    if (ImGui::DragFloat3("Rotation", *rotation, 0.1)) {
        glm::vec3 newRotation = oldRotation - m_inspectedObject->getRotation();
        m_inspectedObject->rotateYPR(newRotation);
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
