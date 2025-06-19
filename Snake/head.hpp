#pragma once

#include "../RayTools/raytools.hpp"

class Head{
private:
    bool isAlive;
    Vector2 position;
    Vector2 collisionPoint;
    uint radius;
    float angleOfMovement;
    Color color;
    Vector2 eyes[2];
    Vector2 pupils[2];
    Vector2 nostrils[2];
    Vector4 deadLines[4];

    void CalculateSymmetricOffsets(Vector2 objects[2], float offsetX, float offsetY);
    void UpdateDeadEyes();
    void RotatePupils(Vector2 pupilsFollowTarget);
public:
    Head(const Vector2 &startPosition, const uint radius, const float startAngleOfMovement, const Color color);
    ~Head() = default;
    
    void Kill();
    bool IsAlive() const;
    Vector2 GetCollisionPoint() const;
    void Update(const Vector2 &newPosition, const float newAngle, const Vector2 &pupilsFollowTarget);
    void Draw() const;
};