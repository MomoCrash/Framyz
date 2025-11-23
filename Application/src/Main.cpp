#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "engine.hpp"
#include "asset/AssetLoader.h"

int main() {
    RenderSystem* render = GameManager::AddSystem<RenderSystem>();
    AssetLoader::loadRenderAsset(render);
    

#ifdef FRAMYZ_EDITOR
    EditorSystem* editor = GameManager::AddSystem<EditorSystem>();
    editor->AddRender(render);
#endif

    PhysicSystem* physics = GameManager::AddSystem<PhysicSystem>();

    GameManager::Run();
    GameManager::Destroy();
    
    return 0;
}
