#pragma once

#include "../IEditorWindow.h"

struct EditorSystem;
class EntityManager;

class HierarchyWindow final : IEditorWindow {
public:
    explicit HierarchyWindow(EditorSystem* editor);

    void open() override;
    void close() override;
    void draw() override;

private:
    
    EntityManager * m_entityManager;
    EditorSystem *  m_editorSystem;
};
