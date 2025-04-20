#include "Food.hpp"

Food::Food(const uint size, const Vector2 position) :
    size(size),
    rotation(0.0f),
    position(position){}

void Food::LoadTexture(const char* filePath){
    texture = LoadTextureFromImage(LoadImage(filePath));
}

uint Food::GetRadius() const{ return size; }
Vector2 Food::GetPosition() const{ return position; }

void Food::Update(){
    
}

void Food::Draw() const{
    DrawTexturePro(
        texture,
        {0.0f, 0.0f, (float)texture.width, (float)-texture.height},
        {position.x, position.y, (float)size, (float)size},
        {(float)size / 2.0f, (float)size / 2.0f},
        rotation,
        WHITE
    );
}