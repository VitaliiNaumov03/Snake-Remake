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

Texture2D ResourceManager::LoadT(const char *filePath){
    Texture2D texture = LoadTexture(filePath);
    if (!IsTextureValid(texture))
        throw std::runtime_error(std::string("Unable to load texture from ") + filePath);
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
    return texture;
}

void ResourceManager::LoadAllTextures(){
    textureMap.reserve(4);
    textureMap.emplace(TextureID::Food, LoadT("Resources/apple.png"));
    textureMap.emplace(TextureID::BigFood, LoadT("Resources/golden apple.png"));
    textureMap.emplace(TextureID::Poison, LoadT("Resources/poison.png"));
    // textureMap.emplace(TextureID::Background, LoadT("Resources/apple.png"));
}

const Texture2D &ResourceManager::Get(TextureID id) const{
    auto iterator = textureMap.find(id);
    if (iterator != textureMap.end())
        return iterator->second;
    throw std::runtime_error("ResourceManager: Requested texture was not found");
}