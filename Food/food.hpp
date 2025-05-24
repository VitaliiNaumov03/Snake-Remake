#pragma once

#include "../RayTools/raytools.hpp"
#include <stdexcept>

#define APPEARANCE_SPEED (size * 6)
#define ROTATION_SPEED (size * 9)
#define IDLE_TIME_S 3.0f
#define IDLE_AMPLITUDE 0.15f //Amplitude of oscillations in "Idle" state (15% from size)
#define IDLE_FREQUENCY 7.0f //Frequency of oscillations in "Idle" state (radians/second)

//For BigFood and Poison
#define MIN_UNACTIVE_TIME_S 7
#define MAX_UNACTIVE_TIME_S 15
#define GET_UNACTIVE_TIME_S (float)GetRandomValue(MIN_UNACTIVE_TIME_S, MAX_UNACTIVE_TIME_S)

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

class Poison : public Food{
private:
    float unactiveTimeS;
    Stopwatch rotationStopwatch;

    void Rotate();
    void AppearingAnimation() override;
    void DisappearingAnimation() override;
    void IdleAnimation() override;
    void UnactiveLogic() override;
public:
    Poison(const Texture2D *texture, const uint eatingPoins, const uint size, const float rotation);
    ~Poison() override = default;
};