// Created by Ethan G. on 22/06/2025.

#include "EditorSystem.h"

#include "../EntityFactory.h"
#include "../GameManager.h"
#include "../interfaces/editor/HierarchyWindow.h"

EditorSystem::~EditorSystem() {
    for (auto openedWindows : m_editorWindows) {
        openedWindows->clear();
        openedWindows->close();
        delete openedWindows;
    }
    
    m_editorWindows.clear();

    m_nodeEditor->clear();
    m_nodeEditor->close();
    
    delete m_gui;
}

void EditorSystem::create() {

    BaseSystem::create();
    
    RenderDevice::Initialize("Application");
    
    m_gui = new GuiHandler();
    m_guiIndex = m_gui->inject(m_renderSystem->Window);
    
    InspectorWindow* inspectorWindow = new InspectorWindow();
    AttachWindow(inspectorWindow);
    
    HierarchyWindow* hierarchyWindow = new HierarchyWindow();
    AttachWindow(hierarchyWindow);
    
    SceneWindow* sceneWindow = new SceneWindow();
    sceneWindow->setRenderWindow(m_renderSystem);
    AttachWindow(sceneWindow);
    
    m_nodeEditor = new NodeEditor(m_gui);

    for (auto & editorWindow : m_editorWindows) {
        editorWindow->create();
        editorWindow->open();
    }
    
}

void EditorSystem::update() {
}

void EditorSystem::drawWindow() {
    BaseSystem::drawWindow();

    m_gui->setContext(m_guiIndex);
    
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    // If so, get the main viewport:
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    // Set the parent window's styles to match that of the main viewport:
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

    // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    
    ImGui::PopStyleVar(3);

    // Check if Docking is enabled:
    ImGuiIO& io = ImGui::GetIO();
    ImGuiID dockspace_id = ImGui::GetID("MainDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {

        if (ImGui::BeginMenu("File"))
        {
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            if (ImGui::MenuItem("Nodyz Editor")) {
                m_nodeEditor->open();
            }
            ImGui::EndMenu();
            
        }


        ImGui::EndMenuBar();

    }
    
    ImGui::End();

    for (auto& attachedWindows : m_editorWindows) {
        attachedWindows->draw();
    }

    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    
    ImGui_ImplVulkan_RenderDrawData(draw_data, m_renderSystem->Window->getRenderContext().getCommandBuffer());

    m_nodeEditor->draw();
}

void EditorSystem::AddRender(RenderSystem *system) {
    m_renderSystem = system;
}

void EditorSystem::AttachWindow(IEditorWindow *window) {
    m_editorWindows.push_back(window);
}

NodeEditor * EditorSystem::getNodeEditor() {
    return m_nodeEditor;
}
