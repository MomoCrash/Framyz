#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "engine.hpp"
#include "asset/AssetLoader.h"

int main() {
    RenderSystem* render = GameManager::AddSystem<RenderSystem>();
    AssetLoader::loadRenderAsset(render);
    
    PhysicSystem* physics = GameManager::AddSystem<PhysicSystem>();
    

#ifdef FRAMYZ_EDITOR
    EditorSystem* editor = GameManager::AddSystem<EditorSystem>();
    editor->AddRender(render);
#endif

    GameManager::Run();
    GameManager::Destroy();
    return 0;
}
