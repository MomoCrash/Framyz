// Created by Ethan G. on 06/10/2025.

#include "AssetLoader.h"

AssetLoader & AssetLoader::GetInstance() {
    static AssetLoader Instance;
    return Instance;
}

bool AssetLoader::loadRenderAsset(RenderSystem const* render) {
    
    RenderAssets[Asset::Render::CUBE] = new Mesh(render->Window->getRenderContext(),
        GeometryFactory::GetPrimitive(Primitive::CUBE));

    return true;
    
}

Mesh * AssetLoader::getRenderAsset(const Asset::Render render) {
    return RenderAssets.at(render);
}
