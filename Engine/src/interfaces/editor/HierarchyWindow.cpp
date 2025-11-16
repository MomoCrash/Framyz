#include "HierarchyWindow.h"

#include "../../GameManager.h"
#include "../../systems/EditorSystem.h"
#include "../../EntityFactory.h"

HierarchyWindow::HierarchyWindow(EditorSystem *editor) {
    
    m_editorSystem = editor;
    m_entityManager = &GameManager::GetEntityManager();
    
}

void HierarchyWindow::open() {
}

void HierarchyWindow::close() {
}

void HierarchyWindow::draw() {
    ImGui::Begin("Hierarchy Window");
    
    ImGui::SeparatorText("Hierarchy");
    if (ImGui::Button("Create entity")) {
        Entity* entity = EntityFactory::CreateEntity();
        EntityFactory::AttachComponent(ComponentType::MeshRenderer, entity);
        
        m_editorSystem->getInspector()->setInspectedObject(entity);
    }
    
    for (int i = 0; i < m_entityManager->getEntityCount(); i++) {
        if (ImGui::Button(std::to_string(i).c_str(), ImVec2(ImGui::GetWindowWidth()/4, 20))) {
            m_editorSystem->getInspector()->setInspectedObject(m_entityManager->getEntity(i));
        }
        ImGui::Spacing();
    }
    
    ImGui::End();
    
}
