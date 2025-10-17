// Created by Ethan G. on 06/10/2025.

#include "AssetLoader.h"

#include "Engine/systems/RenderSystem.h"

AssetLoader & AssetLoader::GetInstance() {
    static AssetLoader Instance;
    return Instance;
}

bool AssetLoader::loadRenderAsset(RenderSystem* render) {
    
    GetInstance().RenderAssets[Asset::Render::CUBE] = new Mesh("Cube", render->Window->getRenderContext(),
        GeometryFactory::GetPrimitive(Primitive::CUBE));

    GetInstance().RenderAssets[Asset::Render::PLANE] = new Mesh("Plane", render->Window->getRenderContext(),
    GeometryFactory::GetPrimitive(Primitive::PLANE));

    return true;
    
}

Mesh * AssetLoader::getRenderAsset(Asset::Render render) {
    return GetInstance().RenderAssets.at(render);
}
