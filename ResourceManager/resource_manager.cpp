#include "resource_manager.hpp"

#define FOOD "Resources/apple.png"
#define BIG_FOOD "Resources/golden apple.png"
#define POISON "Resources/poison.png"
#define BACKGROUND_TOP "Resources/background top.png"
#define BACKGROUND_BOTTOM "Resources/background bottom.png"

ResourceManager::~ResourceManager(){
    for (auto &[id, texture] : textureMap){
        if (IsTextureValid(texture))
            UnloadTexture(texture);
    }
    textureMap.clear();

    if (!imageMap.empty()){
        for (auto &[id, image] : imageMap){
            if (IsImageValid(image))
                UnloadImage(image);
        }
        imageMap.clear();
    }
}

ResourceManager &ResourceManager::GetInstance(){
    static ResourceManager instance;
    return instance;
}

Image ResourceManager::LoadI(const std::string &fileName){
    if (!FileExists(fileName.c_str()))
        throw std::runtime_error(fileName + " doesn't exist");
        
    Image image = LoadImage(fileName.c_str());

    if (!IsImageValid(image))
        throw std::runtime_error(fileName + "is not valid");

    return image;
}

void ResourceManager::PreloadGameImages(){
    std::lock_guard<std::mutex> lock(mtx);
    imageMap.reserve(5);
    imageMap[TextureID::Food] = LoadI(FOOD);
    imageMap[TextureID::BigFood] = LoadI(BIG_FOOD);
    imageMap[TextureID::Poison] = LoadI(POISON);
    imageMap[TextureID::BackgroundTop] = LoadI(BACKGROUND_TOP);
    imageMap[TextureID::BackgroundBottom] = LoadI(BACKGROUND_BOTTOM);
}

void ResourceManager::LoadTexturesFromImages(TextureFilter filter){
    textureMap.reserve(imageMap.size());
    for (auto &[id, image] : imageMap){
        Texture2D texture = LoadTextureFromImage(image);
        GenTextureMipmaps(&texture);
        SetTextureFilter(texture, filter);
        SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);

        textureMap[id] = texture;
        UnloadImage(image);
    }
    imageMap.clear();
}

void ResourceManager::LoadTextureFromFile(TextureID id, const std::string &fileName, TextureFilter filter){
    if (!FileExists(fileName.c_str()))
        throw std::runtime_error(fileName + " doesn't exist");

    std::unique_lock<std::mutex> lock(mtx);

    if (textureMap.find(id) == textureMap.end()){ //If texture hasn't been loaded yet
        lock.unlock();
        Texture2D texture = LoadTexture(fileName.c_str());
        if (!IsTextureValid(texture))
            throw std::runtime_error(std::string("Unable to load texture from ") + fileName);
        GenTextureMipmaps(&texture);
        SetTextureFilter(texture, filter);
        SetTextureWrap(texture, TEXTURE_WRAP_CLAMP);
        lock.lock();
        textureMap.emplace(id, texture);
    }
}

void ResourceManager::UnloadT(TextureID id){
    std::lock_guard<std::mutex> lock(mtx);
    auto it = textureMap.find(id);
    if (it != textureMap.end()){
        UnloadTexture(it->second);
        textureMap.erase(it);
    }
}

const Texture2D &ResourceManager::Get(TextureID id) const{
    std::lock_guard<std::mutex> lock(mtx);
    auto iterator = textureMap.find(id);
    if (iterator != textureMap.end())
        return iterator->second;
    throw std::runtime_error("ResourceManager: Requested texture was not found");
}