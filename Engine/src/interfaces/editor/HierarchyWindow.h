#pragma once

#include "../IEditorWindow.h"

struct EditorSystem;
class EntityManager;

class HierarchyWindow final : public IEditorWindow {
public:
    explicit HierarchyWindow();

    void create() override;
    void clear() override;

    void open() override;
    void close() override;
    void draw() override;

private:
    
    EntityManager * m_entityManager;
    EditorSystem *  m_editorSystem;
};
