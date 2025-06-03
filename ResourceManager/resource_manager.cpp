#include "resource_manager.hpp"

ResourceManager::~ResourceManager(){
    for (auto& [id, texture] : textureMap)
        UnloadTexture(texture);
    textureMap.clear();
}

ResourceManager &ResourceManager::GetInstance(){
    static ResourceManager instance;
    return instance;
}

void ResourceManager::LoadT(TextureID id, const std::string fileName, TextureFilter filter){
    if (!FileExists(fileName.c_str()))
        throw std::runtime_error(fileName + " doesn't exist");
    
    if (textureMap.find(id) == textureMap.end()){ //If texture hasn't been loaded yet
        Texture2D texture = LoadTexture(fileName.c_str());
        if (!IsTextureValid(texture))
            throw std::runtime_error(std::string("Unable to load texture from ") + fileName);
        GenTextureMipmaps(&texture);
        SetTextureFilter(texture, filter);
        SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
        textureMap.emplace(id, texture);
    }
}

void ResourceManager::UnloadT(TextureID id){
    auto it = textureMap.find(id);
    if (it != textureMap.end()){
        UnloadTexture(it->second);
        textureMap.erase(it);
    }
}

void ResourceManager::LoadGameTextures(){
    textureMap.reserve(5);
    LoadT(TextureID::Food, "Resources/apple.png", TEXTURE_FILTER_BILINEAR);
    LoadT(TextureID::BigFood, "Resources/golden apple.png", TEXTURE_FILTER_BILINEAR);
    LoadT(TextureID::Poison, "Resources/poison.png", TEXTURE_FILTER_BILINEAR);
    LoadT(TextureID::BackgroundTop, "Resources/background top.png", TEXTURE_FILTER_BILINEAR);
    LoadT(TextureID::BackgroundBottom, "Resources/background bottom.png", TEXTURE_FILTER_BILINEAR);
}

const Texture2D &ResourceManager::Get(TextureID id) const{
    auto iterator = textureMap.find(id);
    if (iterator != textureMap.end())
        return iterator->second;
    throw std::runtime_error("ResourceManager: Requested texture was not found");
}