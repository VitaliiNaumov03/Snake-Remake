#pragma once

#include "../RayTools/raytools.hpp"
#include <unordered_map>
#include <memory>
#include <functional>
#include <stdexcept>

//Made using Singleton pattern
class ResourceManager{
public:
    enum class TextureID{ Food, BigFood, Poison, BackgroundTop, BackgroundBottom };
    static ResourceManager &GetInstance();
    void LoadF();
    void UnloadF();
    void LoadAllTextures();
    const Texture2D &Get(TextureID id) const;
    const Font &GetFont() const;
private:
    ResourceManager() = default;
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager &operator=(const ResourceManager&) = delete;

    std::unique_ptr<Font, std::function<void(Font*)>> font;
    std::unordered_map<TextureID, Texture2D> textureMap;
    Texture2D LoadT(const std::string fileName);
};