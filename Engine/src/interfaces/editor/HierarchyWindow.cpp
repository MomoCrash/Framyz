#include "HierarchyWindow.h"

#include "../../GameManager.h"
#include "../../systems/EditorSystem.h"
#include "../../EntityFactory.h"

HierarchyWindow::HierarchyWindow() {
    
    m_editorSystem  = GameManager::GetSystem<EditorSystem>();
    m_entityManager = &GameManager::GetEntityManager();
    
}

void HierarchyWindow::clear() {
}

void HierarchyWindow::create() {
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
        
        m_editorSystem->getWindowByType<InspectorWindow>()->setInspectedObject(entity);
    }
    
    for (int i = 0; i < m_entityManager->getEntityCount(); i++) {
        Entity* entity = m_entityManager->getEntity(i);
        if (entity->isDebugOnly()) continue;
        if (ImGui::Button(std::to_string(i).c_str(), ImVec2(ImGui::GetWindowWidth()/4, 20))) {
            m_editorSystem->getWindowByType<InspectorWindow>()->setInspectedObject(entity);
        }
        ImGui::Spacing();
    }
    
    ImGui::End();
    
}
