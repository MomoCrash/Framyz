// Created by Ethan G. on 06/10/2025.

#ifndef ASSETLOADER_H
#define ASSETLOADER_H
#include <map>

struct RenderSystem;
class Mesh;

class Asset {
    
public:
    enum class Render {
        CUBE,
        PLANE,
    };

};

class AssetLoader {
public:
    static AssetLoader& GetInstance();
    
    static bool loadRenderAsset(RenderSystem*) ;
    static Mesh* getRenderAsset(Asset::Render render);

private:
    std::map<Asset::Render, Mesh*> RenderAssets;
};



#endif //ASSETLOADER_H
