// Created by Ethan G. on 06/10/2025.

#include "AssetLoader.h"

#include "Engine/systems/RenderSystem.h"

AssetLoader & AssetLoader::GetInstance() {
    static AssetLoader Instance;
    return Instance;
}

bool AssetLoader::loadRenderAsset(RenderSystem* render) {
    
    GetInstance().RenderAssets[Asset::Render::CUBE] = new Mesh(render->Window->getRenderContext(),
        GeometryFactory::GetPrimitive(Primitive::CUBE));

    return true;
    
}

Mesh * AssetLoader::getRenderAsset(Asset::Render render) {
    return GetInstance().RenderAssets.at(render);
}
