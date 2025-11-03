#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "engine.hpp"


int main() {

    //test_app();

    RenderSystem* render = GameManager::AddSystem<RenderSystem>();
    AssetLoader::loadRenderAsset(render);

#ifdef FRAMYZ_EDITOR
    EditorSystem* editor = GameManager::AddSystem<EditorSystem>();
    editor->AddRender(render);
#endif

    GameManager::Run();
    
    return 0;
    
}
