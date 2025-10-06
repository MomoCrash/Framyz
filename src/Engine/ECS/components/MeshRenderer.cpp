// Created by Ethan G. on 22/06/2025.

#include "MeshRenderer.h"

#include "Engine/GameManager.h"
#include "Engine/asset/AssetLoader.h"
#include "Engine/systems/RenderSystem.h"

MeshRenderer::MeshRenderer() : Object(nullptr) {
}

void MeshRenderer::display() {
    Component::display();

    ImGui::Text("MeshRenderer");

    
}

void MeshRenderer::instantiate() {
    Component::instantiate();
    
    Object = new RenderObject(AssetLoader::getRenderAsset(Asset::Render::CUBE));
    Object->setTransform(&Owner->getMatrix());
}
