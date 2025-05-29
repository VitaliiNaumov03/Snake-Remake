#include "tongue.hpp"

#define TIME_UNACTIVE_S 3.0f //How many seconds the tongue is not visible

Tongue::Tongue(const Vector2 &headPosition, const uint length, const float startAngleOfMovement, const uint speed) :
    speed(speed),
    active(false),
    position(headPosition),
    distanceFromHead(0),
    angleOfMovement(startAngleOfMovement){
    
    //TEXTURE PARAMETERS
    const Color color = PINK;
    const float bifurcationPoint = length / 2.0f;
    const uint lineWidth = roundf(length / 4.0f);
    const uint height = length;

    //CREATING TONGUE TEXTURE
    const Vector2 topPoint = {
        (float)length,
        0.0f
    };
    const Vector2 bottomPoint = {
        (float)length,
        (float)height
    };
    tongueTexture = LoadRenderTexture(length, height);
    BeginTextureMode(tongueTexture);
        ClearBackground(Color{ color.r, color.g, color.b, 0 });
        const float halfHeight = height / 2.0f;
        DrawLineEx({0.0f, halfHeight}, {bifurcationPoint, halfHeight}, lineWidth, color);
        DrawLineEx({bifurcationPoint, halfHeight}, {topPoint.x, topPoint.y}, lineWidth, color);
        DrawLineEx({bifurcationPoint, halfHeight}, {bottomPoint.x, bottomPoint.y}, lineWidth, color);
    EndTextureMode();
    SetTextureWrap(tongueTexture.texture, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(tongueTexture.texture, TEXTURE_FILTER_BILINEAR);
}

void Tongue::UpdateActive(const Vector2 &newHeadPosition, const float newAngle){
    angleOfMovement = newAngle;
    
    const Vector2 direction = { cosf(angleOfMovement), sinf(angleOfMovement) };
    const float topPoint = roundf(1.5f * tongueTexture.texture.width); //Max forward offset

    distanceFromHead += speed * GetFrameTime();
    position = {
        newHeadPosition.x + distanceFromHead * direction.x,
        newHeadPosition.y + distanceFromHead * direction.y
    };

    if (distanceFromHead >= topPoint){ //If the tongue is in front of the head
        speed = -speed;
        position = { //Preventing from going further than max offset
            newHeadPosition.x + topPoint * direction.x,
            newHeadPosition.y + topPoint * direction.y
        };
        distanceFromHead = topPoint;
    }
    else if (distanceFromHead <= 0.0f){ //If the tongue is in the head position or behind it
        speed = abs(speed);
        active = false;
        position = newHeadPosition;
        distanceFromHead = 0.0f;
    }
}

void Tongue::Update(const Vector2 &newHeadPosition, const float newAngle){
    if (active){
        UpdateActive(newHeadPosition, newAngle);
    }
    else{
        stopwatch.Tick();
        if (stopwatch.GetElapsedTimeS() >= TIME_UNACTIVE_S){
            active = true;
            stopwatch.Reset();
            UpdateActive(newHeadPosition, newAngle);
        }
    }
}

void Tongue::UpdateDead(const Vector2 &newHeadPosition, const float newAngle){
    angleOfMovement = newAngle;
    position.x = newHeadPosition.x + distanceFromHead * cosf(angleOfMovement);
    position.y = newHeadPosition.y + distanceFromHead * sinf(angleOfMovement);
}

void Tongue::Draw() const{
    if (active){
        DrawTexturePro(tongueTexture.texture,
            {0.0f, 0.0f, (float)tongueTexture.texture.width, (float)-tongueTexture.texture.height},
            {position.x, position.y, (float)tongueTexture.texture.width, (float)tongueTexture.texture.height},
            {tongueTexture.texture.width / 2.0f, tongueTexture.texture.height / 2.0f},
            angleOfMovement * RAD_TO_DEG,
            WHITE
        );
    }
}