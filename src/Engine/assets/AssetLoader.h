// Created by Ethan G. on 06/10/2025.

#ifndef ASSETLOADER_H
#define ASSETLOADER_H
#include "Engine/systems/RenderSystem.h"

class Asset {
    
public:
    enum class Render {
        CUBE,

        
    };

};


class AssetLoader {
public:
    static AssetLoader& GetInstance();
    
    bool loadRenderAsset(RenderSystem const*) ;
    Mesh* getRenderAsset(Asset::Render render);

private:
    std::map<Asset::Render, Mesh*> RenderAssets;
};



#endif //ASSETLOADER_H
