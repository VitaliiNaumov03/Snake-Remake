#pragma once

#include "../RayTools/raytools.hpp"
#include <unordered_map>
#include <stdexcept>
#include <mutex>

//Made using Singleton pattern
class ResourceManager{
public:
    enum class TextureID{ Logo, Food, BigFood, Poison, BackgroundTop, BackgroundBottom };
    static ResourceManager &GetInstance();
    void PreloadGameImages();
    void LoadTexturesFromImages(TextureFilter filter);
    void LoadTextureFromFile(TextureID id, const std::string &fileName, TextureFilter filter);
    void UnloadT(TextureID id);
    const Texture2D &Get(TextureID id) const;
private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager &operator=(const ResourceManager&) = delete;

    void LoadI(TextureID id, const std::string &fileName);
    std::unordered_map<TextureID, Texture2D> textureMap;
    std::unordered_map<TextureID, Image> imageMap;
    mutable std::mutex mtx;
};