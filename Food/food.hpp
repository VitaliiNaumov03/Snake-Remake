#pragma once

#include "../RayTools/raytools.hpp"
#include <stdexcept>

#define APPEARANCE_SPEED (size * 6)
#define IDLE_AMPLITUDE 0.15f //Amplitude of oscillations in "Idle" state (15% from size)
#define IDLE_FREQUENCY 7.0f //Frequency of oscillations in "Idle" state (radians/second)

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
    virtual ~Food() = default;
    
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

class BigFood : public Food{
private:
    float unactiveTimeS;

    void IdleAnimation() override;
    void UnactiveLogic() override;
public:
    BigFood(const Texture2D *texture, const uint eatingPoins, const uint size);
    ~BigFood() override = default;
};