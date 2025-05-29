#include "resource_manager.hpp"

ResourceManager::~ResourceManager(){
    for (auto& [id, texture] : textureMap)
        UnloadTexture(texture);
    textureMap.clear();
    UnloadF();
}

ResourceManager &ResourceManager::GetInstance(){
    static ResourceManager instance;
    return instance;
}

Texture2D ResourceManager::LoadT(const std::string fileName){
    if (!FileExists(fileName.c_str()))
        throw std::runtime_error(fileName + " doesn't exist");
    
    Texture2D texture = LoadTexture(fileName.c_str());
    if (!IsTextureValid(texture))
        throw std::runtime_error(std::string("Unable to load texture from ") + fileName);
    GenTextureMipmaps(&texture);
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
    SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
    return texture;
}

void ResourceManager::LoadF(){
    if (font) return;
    auto deleter = [](Font *font){
            if (font){
                UnloadFont(*font);
                delete font;
            }
        };
    font = std::unique_ptr<Font, decltype(deleter)>(
        new Font(LoadFontEx("Resources/LilitaOne.ttf", 64, nullptr, 0)),
        deleter);
    SetTextureFilter(font->texture, TEXTURE_FILTER_BILINEAR);
}

void ResourceManager::UnloadF(){ font.reset(); }

void ResourceManager::LoadAllTextures(){
    textureMap.reserve(5);
    textureMap.emplace(TextureID::Food, LoadT("Resources/apple.png"));
    textureMap.emplace(TextureID::BigFood, LoadT("Resources/golden apple.png"));
    textureMap.emplace(TextureID::Poison, LoadT("Resources/poison.png"));
    textureMap.emplace(TextureID::BackgroundTop, LoadT("Resources/background top.png"));
    textureMap.emplace(TextureID::BackgroundBottom, LoadT("Resources/background bottom.png"));
}

const Texture2D &ResourceManager::Get(TextureID id) const{
    auto iterator = textureMap.find(id);
    if (iterator != textureMap.end())
        return iterator->second;
    throw std::runtime_error("ResourceManager: Requested texture was not found");
}

const Font &ResourceManager::GetFont() const{
    if (!font)
        throw std::runtime_error("ResourceManager: Requested font was not found");
    else
        return *font;
}