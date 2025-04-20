#pragma once

#include "../RayTools/raytools.hpp"

class Food{
private:
    uint size;
    float rotation;
    Vector2 position;
    Texture2D texture;
public:
    Food(const uint size, const Vector2 position);
    ~Food(){ UnloadTexture(texture); };

    void LoadTexture(const char* filePath);
    uint GetRadius() const;
    Vector2 GetPosition() const;
    void Update();
    void Draw() const;
};