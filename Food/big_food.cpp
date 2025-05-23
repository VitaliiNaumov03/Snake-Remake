#include "food.hpp"

#define IDLE_TIME_S 3.0f
#define MIN_UNACTIVE_TIME_S 10
#define MAX_UNACTIVE_TIME_S 20

BigFood::BigFood(const Texture2D *texture, const uint eatingPoins, const uint size) :
    Food(texture, eatingPoins, size),
    unactiveTimeS((float)GetRandomValue(MIN_UNACTIVE_TIME_S, MAX_UNACTIVE_TIME_S)){}

void BigFood::IdleAnimation(){
    animationStopwatch.Tick();
    if (animationStopwatch.GetElapsedTimeS() >= IDLE_TIME_S){
        currState = State::Disappearing;
        return;
    }
    currSize = size * (1.0f + IDLE_AMPLITUDE * sinf(IDLE_FREQUENCY * animationStopwatch.GetElapsedTimeS()));
}

void BigFood::UnactiveLogic(){
    animationStopwatch.Tick();
    if (animationStopwatch.GetElapsedTimeS() >= unactiveTimeS){
        animationStopwatch.Reset();
        unactiveTimeS = (float)GetRandomValue(MIN_UNACTIVE_TIME_S, MAX_UNACTIVE_TIME_S);
        currState = State::Appearing;
    }
}