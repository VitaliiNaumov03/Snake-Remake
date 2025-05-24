#pragma once

#include "../RayTools/raytools.hpp"
#include <unordered_map>
#include <stdexcept>

//Made using Singleton pattern
class ResourceManager{
public:
    enum class TextureID{ Food, BigFood, Poison, Background };
    static ResourceManager &GetInstance();
    void LoadAllTextures();
    const Texture2D &Get(TextureID id) const;
private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager &operator=(const ResourceManager&) = delete;

    std::unordered_map<TextureID, Texture2D> textureMap;
    Texture2D LoadT(const char *fileName);
};