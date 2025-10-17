// Created by Ethan G. on 20/06/2025.

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Engine/engine.hpp"


int main() {

    //test_app();

//     RenderSystem* render = GameManager::AddSystem<RenderSystem>();
//     AssetLoader::loadRenderAsset(render);
//
// #ifdef FRAMYZ_EDITOR
//     EditorSystem* editor = GameManager::AddSystem<EditorSystem>();
//     editor->AddRender(render);
// #endif

    Entity* first = EntityFactory::CreateEntity();
    EntityFactory::CreateEntity();
    EntityFactory::CreateEntity();
    EntityFactory::CreateEntity();
    EntityFactory::CreateEntity();
    //
    // MeshRenderer* mesh = EntityFactory::AttachComponent<MeshRenderer>(first);
    // Mesh* meshData = new Mesh(render->Window->getRenderContext(), GeometryFactory::GetPrimitive(Primitive::CUBE));
    // mesh->Object = new RenderObject(meshData);
    // mesh->Object->setTransform(&first->getMatrix());

    GameManager::Run();
    
    return 0;
    
}
