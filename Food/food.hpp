#pragma once

#include "../RayTools/raytools.hpp"
#include <stdexcept>

class Food{
protected:
    uint eatingPoints; //How many points player gets when snake eats this food
    uint size;
    float currSize;
    float rotation;
    float currRotation;
    Vector2 position;
    const Texture2D *texture;
    enum class State{ Appearing, Disappearing, Idle, Unactive };
    State currState;
    Stopwatch animationStopwatch;

    virtual void AppearingAnimation();
    virtual void DisappearingAnimation();
    virtual void IdleAnimation();
    virtual void UnactiveLogic();
public:
    Food(const Texture2D *texture, const uint eatingPoins, const uint size, const float rotation = 0.0f);
    ~Food() = default;
    
    uint GetRadius() const;
    Vector2 GetPosition() const;
    void GenerateNewPosition();
    bool WillSpawnNow() const;
    bool IsActive() const;
    void Disappear();
    uint Eat();
    void Reset();
    void Update();
    void Draw() const;
};