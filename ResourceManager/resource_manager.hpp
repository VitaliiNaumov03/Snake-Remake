#pragma once

#include "../RayTools/raytools.hpp"
#include <unordered_map>
#include <stdexcept>

//Made using Singleton pattern
class ResourceManager{
public:
    enum class TextureID{ Logo, Food, BigFood, Poison, BackgroundTop, BackgroundBottom };
    static ResourceManager &GetInstance();
    void LoadT(TextureID id, const std::string fileName, TextureFilter filter);
    void UnloadT(TextureID id);
    void LoadGameTextures();
    const Texture2D &Get(TextureID id) const;
private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager &operator=(const ResourceManager&) = delete;

    std::unordered_map<TextureID, Texture2D> textureMap;
};