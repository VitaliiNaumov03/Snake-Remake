#include "food.hpp"

Poison::Poison(const Texture2D *texture, const uint eatingPoins, const uint size, const float rotation) :
    Food(texture, eatingPoins, size, rotation),
    unactiveTimeS(GET_UNACTIVE_TIME_S){}

void Poison::Rotate(){
    static const float cache = ROTATION_SPEED * PI / 180.0f;
    rotationStopwatch.Tick();
    currRotation = rotation * sinf(rotationStopwatch.GetElapsedTimeS() * cache);
}

void Poison::AppearingAnimation(){
    if (currSize < (float)size){
        currSize += APPEARANCE_SPEED * GetFrameTime();
        Rotate();
    }
    else{
        currSize = size;
        currState = State::Idle;
    }
}

void Poison::DisappearingAnimation(){
    if (currSize > 0.0f){
        currSize -= APPEARANCE_SPEED * GetFrameTime();
        Rotate();
    }
    else{
        currSize = 0.0f;
        currRotation = 0.0f;
        animationStopwatch.Reset();
        rotationStopwatch.Reset();
        currState = State::Unactive;
    }
}

void Poison::IdleAnimation(){
    animationStopwatch.Tick();
    if (animationStopwatch.GetElapsedTimeS() >= IDLE_TIME_S){
        currState = State::Disappearing;
        return;
    }
    Rotate();
}

void Poison::UnactiveLogic(){
    animationStopwatch.Tick();
    if (animationStopwatch.GetElapsedTimeS() >= unactiveTimeS){
        animationStopwatch.Reset();
        unactiveTimeS = GET_UNACTIVE_TIME_S;
        currState = State::Appearing;
    }
}