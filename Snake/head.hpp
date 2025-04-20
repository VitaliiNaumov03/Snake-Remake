#pragma once

#include "../RayTools/raytools.hpp"
#include <array>

class Head{
private:
    bool isAlive;
    Vector2 position;
    Vector2 collisionPoint;
    uint radius;
    float angleOfMovement;
    Color color;
    RenderTexture2D headTexture;
    float eyesPosition;
    Vector2 pupils[2];

    const std::array<Vector2, 2> CalculateEyesPosition() const;
    const std::array<Vector2, 2> CalculateNostrilsPosition() const;
    void MakeDeadTexture(const std::array<Vector2, 2> eyes);
    void RotatePupils(const Vector2 &pupilsFollowTarget);
public:
    Head(const Vector2 &startPosition, const uint radius, const float startAngleOfMovement, const Color &color);
    ~Head(){ UnloadRenderTexture(headTexture); };
    
    void Kill();
    bool IsAlive() const;
    Vector2 GetCollisionPoint() const;
    void Update(const Vector2 &newPosition, const float newAngle, const Vector2 &pupilsFollowTarget);
    void Draw() const;
};