#pragma once

#include "../RayTools/raytools.hpp"

class Tongue{
private:
    int speed;
    bool active;
    Vector2 position;
    float distanceFromHead;
    float angleOfMovement;
    Stopwatch stopwatch;
    RenderTexture2D tongueTexture;

    void UpdateActive(const Vector2 &newHeadPosition, const float newAngle);
public:
    Tongue(const Vector2 &headPosition, const uint length, const float startAngleOfMovement, const uint speed);
    ~Tongue(){ UnloadRenderTexture(tongueTexture); };

    void Reset(const Vector2 &headPosition, const float startAngleOfMovement);
    void Update(const Vector2 &newHeadPosition, const float newAngle);
    void UpdateDead(const Vector2 &newHeadPosition, const float newAngle);
    void Draw() const;
};