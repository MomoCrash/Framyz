// Created by Ethan G. on 22/06/2025.

#include "MeshRenderer.h"

#include "Engine/EntityFactory.h"
#include "Engine/asset/AssetLoader.h"
#include "Engine/systems/RenderSystem.h"

MeshRenderer::MeshRenderer() : Object(nullptr) {
}

void MeshRenderer::EDITOR_Display() {
    Component::EDITOR_Display();

    ImGui::SeparatorText("Mesh Renderer Component");
    ImGui::Text(Object->getMesh()->getAssetName());
    ImGui::SameLine(0, 10);
    if (ImGui::Button("Browse")) {
        Object->setMesh(AssetLoader::getRenderAsset(Asset::Render::PLANE));
    }
    ImGui::Spacing();
    if (ImGui::Button("Remove")) {
        EntityFactory::RemoveComponent(this);
    }
    
}

void MeshRenderer::instantiate() {
    Component::instantiate();

    Object = new RenderObject(AssetLoader::getRenderAsset(Asset::Render::CUBE));
    Object->setTransform(&Owner->getMatrix());
}
