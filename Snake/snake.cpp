#include "snake.hpp"

#define LERP_AREA (radius * 3)
#define SHAKES_PER_SECOND 50
#define SHAKE_DURATION_MS 150.0f
#define SHAKE_INTENSITY roundf(radius / 10.0f)

Snake::Snake(Vector2 startPosition, uint startLength, const uint radius, const uint segmentsGap, const uint speed) :
    radius(radius),
    segmentsGap(segmentsGap),
    speed(speed),
    angleOfMovement(3 * HALF_PI),
    currShakeIntensity(0),
    shakeInterval(1.0f / SHAKES_PER_SECOND), //So that the first shake starts without pause
    colContr(&ColorController::GetInstance()),
    head(startPosition, radius, angleOfMovement, colContr->GetColorFor(0)),
    tongue(startPosition, radius, angleOfMovement, speed * 0.4f){

    body.reserve(startLength + 10);
    body.emplace_back(startPosition);
    for (uint i = 1; i < startLength; ++i)
        body.emplace_back((Vector2){startPosition.x, startPosition.y + segmentsGap * i});
}

void Snake::RotateAndMove(Vector2 &origin, const Vector2 &destination, const uint targetDistance){
    const float currDistance = Vector2Distance(origin, destination);
    const float deltaDistance = currDistance - targetDistance;
    if (currDistance > targetDistance){
        const float angle = atan2(destination.y - origin.y, destination.x - origin.x);
        origin.x += deltaDistance * cosf(angle);
        origin.y += deltaDistance * sinf(angle);
    }
}

void Snake::RotateAndMoveHead(Vector2 destination, uint targetDistance, const float speed){
    angleOfMovement = atan2f(destination.y - body[0].y, destination.x - body[0].x);
    //Calculating point on targetDistance from destination
    destination = {
        destination.x + targetDistance * cosf(angleOfMovement + PI), //Using PI to invert angle
        destination.y + targetDistance * sinf(angleOfMovement + PI)
    };
    targetDistance = radius; //Now targetDistance (from head center to destination) is the snake's radius
    
    const float currDistance = Vector2Distance(body[0], destination);

    if (currDistance > targetDistance){ //If distance between mouth and new destination bigger that snake's radius
        const float deltaTime = GetFrameTime();

        if (currDistance <= LERP_AREA){ //In this area, the head moves not at a constant speed, but with a lerp
            static const float lerpSpeed = speed * 0.03f / (radius / 12.0f);
            const float lerpCoeff = 1.0f - expf(-lerpSpeed * deltaTime); //This is needed for FPS independence
            body[0] = Vector2Lerp(body[0], destination, lerpCoeff);
        }
        else{ //If head is not in that area, it moves at a regular speed
            body[0].x += speed * deltaTime * cosf(angleOfMovement);
            body[0].y += speed * deltaTime * sinf(angleOfMovement);
        }
    }
}

uint Snake::GetRadius() const{ return radius; }
uint Snake::GetLength() const{ return body.size(); }
bool Snake::IsAlive() const{ return head.IsAlive(); }

bool Snake::Bites(const Vector2 &circleCenter, const float circleRadius) const{
    return CheckCollisionPointCircle(head.GetCollisionPoint(), circleCenter, circleRadius);
}

bool Snake::BitesItself() const{
    /*Ignore the first 2 segments,
    so that when the snake turns its head backwards, while standing still,
    it doesn't die*/
    for (uint i = 3; i < body.size(); ++i){
        if (Bites(body[i], static_cast<float>(radius)))
            return true;
    }
    return false;
}

bool Snake::BodyCollidesWith(const Vector2 &point) const{
    for (const Vector2 &segment : body){
        if (CheckCollisionPointCircle(point, segment, radius))
            return true;
    }
    return false;
}

void Snake::Grow(const uint numOfNewSegments){
    body.insert(body.end(), numOfNewSegments, body.back());
}

void Snake::Kill(const CauseOfDeath causeOfDeath){
    head.Kill();
    deadPoint = {
        body[0].x + LERP_AREA * cosf(angleOfMovement),
        body[0].y + LERP_AREA * sinf(angleOfMovement)
    };
    this->causeOfDeath = causeOfDeath;
    if (causeOfDeath == CauseOfDeath::BitItself) currShakeIntensity = SHAKE_INTENSITY;
}

void Snake::Update(const Vector2 &destination, const uint targetDistance, const Vector2 &pupilsFollowTarget){
    //Body
    RotateAndMoveHead(destination, targetDistance, speed);
    for (uint i = 1; i < body.size(); ++i)
        RotateAndMove(body[i], body[i - 1], segmentsGap);
    
    //Head
    head.Update(body[0], angleOfMovement, pupilsFollowTarget);
    tongue.Update(body[0], angleOfMovement);
}

//Returns false when animation is finished
bool Snake::UpdateDead(){
    switch (causeOfDeath){
        case CauseOfDeath::AtePoison:{
            if (static_cast<uint>(Vector2Distance(body[0], deadPoint)) == radius) return false;

            RotateAndMoveHead(deadPoint, 0, speed);
            for (uint i = 1; i < body.size(); ++i)
                RotateAndMove(body[i], body[i - 1], segmentsGap);
            
            head.Update(body[0], angleOfMovement, {0.0f, 0.0f});
            tongue.UpdateDead(body[0], angleOfMovement);
        
            return true;
        }

        case CauseOfDeath::BitItself:{
            if (shakeAnimation.GetElapsedTimeMs() >= SHAKE_DURATION_MS){
                currShakeIntensity = 0;

                //Reset positions back to normal
                head.Update(body[0], angleOfMovement, {0.0f, 0.0f});
                tongue.UpdateDead(body[0], angleOfMovement);

                return false;
            }
            else{
                shakeAnimation.Tick();
                shakeInterval.Tick();
                if (shakeInterval.GetElapsedTimeS() >= 1.0f / SHAKES_PER_SECOND){
                    head.Update({body[0].x + currShakeIntensity, body[0].y}, angleOfMovement, {0.0f, 0.0f});
                    tongue.UpdateDead({body[0].x + currShakeIntensity, body[0].y}, angleOfMovement);
                    currShakeIntensity *= -1;
                    shakeInterval.Reset();
                }
                return true;
            }
        }
        
        default: return false;
    }
}

void Snake::Draw() const{
    //Body
    for (int i = body.size() - 1; i > 0; --i)
        DrawCircle(body[i].x + currShakeIntensity, body[i].y, radius, colContr->GetColorFor(i));
    
    //Head
    tongue.Draw();
    head.Draw();
}