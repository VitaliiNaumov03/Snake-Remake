#include "food.hpp"

Food::Food(const Texture2D *texture, const uint eatingPoins, const uint size, const float rotation) :
    eatingPoints(eatingPoins),
    size(size),
    currSize(0),
    rotation(rotation),
    currRotation(0),
    texture(texture),
    currState(State::Unactive){}

uint Food::GetRadius() const{ return static_cast<uint>(roundf(size / 2.0f)); }
Vector2 Food::GetPosition() const{ return position; }

void Food::AppearingAnimation(){
    static const uint speed = APPEARANCE_SPEED;
    if (currSize < (float)size){
        currSize += speed * GetFrameTime();
    }
    else{
        currSize = size;
        currState = State::Idle;
    }
}

void Food::DisappearingAnimation(){
    static const uint speed = APPEARANCE_SPEED;
    if (currSize > 0){
        currSize -= speed * GetFrameTime();
    }
    else{
        currSize = 0;
        animationStopwatch.Reset();
        currState = State::Unactive;
    }
}

void Food::IdleAnimation(){
    animationStopwatch.Tick();
    static const float resetPeriod = 2.0f * PI / IDLE_FREQUENCY; //Stopwatch reset period to avoid float overflow
    if (animationStopwatch.GetElapsedTimeS() >= resetPeriod)
        animationStopwatch.Reset();
    
    currSize = size * (1.0f + IDLE_AMPLITUDE * sinf(IDLE_FREQUENCY * animationStopwatch.GetElapsedTimeS()));
}

void Food::UnactiveLogic(){ currState = State::Appearing; }

void Food::GenerateNewPosition(){
    position = {
        (float)GetRandomValue(GetRadius(), GetScreenWidth() - GetRadius()),
        (float)GetRandomValue(GetRadius(), GetScreenHeight() - GetRadius())
    };
}

bool Food::WillSpawnNow() const{
    return currSize == 0 && currState == State::Appearing;
}

bool Food::IsActive() const{
    return currState == State::Appearing || currState == State::Idle;
}

void Food::Disappear(){
    currState = State::Disappearing;
}

uint Food::Eat(){
    Disappear();
    return eatingPoints;
}

void Food::Reset(){
    currSize = 0;
    currState = State::Unactive;
}

void Food::Update(){
    switch (currState){
        case State::Appearing:
            AppearingAnimation();
            break;

        case State::Disappearing:
            DisappearingAnimation();
            break;

        case State::Idle:
            IdleAnimation();
            break;

        case State::Unactive:
            UnactiveLogic();
            break;
    }
}

void Food::Draw() const{
    if (currState != State::Unactive){
        DrawTexturePro(
            *texture,
            {0.0f, 0.0f, (float)texture->width, (float)texture->height},
            {position.x, position.y, (float)currSize, (float)currSize},
            {(float)currSize / 2.0f, (float)currSize / 2.0f},
            currRotation,
            WHITE
        );
    }
}