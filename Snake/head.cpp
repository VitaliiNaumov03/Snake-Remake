#include "head.hpp"

float eyesOffsetX;
float eyesOffsetY;
float eyesBackRadius;
float eyesRadius;
float pupilsRadius;
float nostrilsRadius;

Head::Head(const Vector2 &startPosition, const uint radius, const float startAngleOfMovement, const Color color) :
    isAlive(true),
    position(startPosition),
    radius(radius),
    angleOfMovement(startAngleOfMovement),
    color(color){
    
    //Eyes offset from center of head
    eyesOffsetX = roundf(radius * 2.0f / 3.0f);
    eyesOffsetY = radius / 2.0f;

    //Radiuses
    eyesBackRadius = roundf(radius / 1.5f);
    eyesRadius = roundf(eyesBackRadius / 2.0f);
    pupilsRadius = eyesRadius / 1.6f;
    nostrilsRadius = roundf(radius * 0.0625f);

    //EYES
    CalculateSymmetricOffsets(eyes, eyesOffsetX, eyesOffsetY);
    //Initializing base positions before rotation
    pupils[0] = eyes[0];
    pupils[1] = eyes[1];
    RotatePupils({ //Rotates forward
        position.x + radius * 2.0f * cosf(angleOfMovement),
        position.y + radius * 2.0f * sinf(angleOfMovement)
    });
    
    //NOSTRILS
    CalculateSymmetricOffsets(nostrils, eyesOffsetY, -eyesOffsetY);
}

void Head::CalculateSymmetricOffsets(Vector2 objects[2], float offsetX, float offsetY){
    const float cosAngle = cosf(angleOfMovement);
    const float sinAngle = sinf(angleOfMovement);
    
    //Forward displacement vector in the direction of movement
    Vector2 forwardVec = {offsetY * cosAngle, offsetY * sinAngle};
    
    //Side displacement vector (perpendicular to the direction of movement)
    Vector2 sideVec = {offsetX * sinAngle, -offsetX * cosAngle};
    
    objects[0] = position - forwardVec + sideVec;
    objects[1] = position - forwardVec - sideVec;
}

void Head::UpdateDeadEyes(){
    float angle45 = angleOfMovement + (PI / 4.0f); //45°
    float angle135 = angleOfMovement + (3 * PI / 4.0f); //135°
    float angle225 = angleOfMovement + (5 * PI / 4.0f); //225°
    float angle315 = angleOfMovement + (7 * PI / 4.0f); //315°

    //Normalizing angles to the range [0, 2π)
    angle45 = fmodf(angle45 + 2 * PI, 2 * PI);
    angle135 = fmodf(angle135 + 2 * PI, 2 * PI);
    angle225 = fmodf(angle225 + 2 * PI, 2 * PI);
    angle315 = fmodf(angle315 + 2 * PI, 2 * PI);

    uint l = 0;
    for (uint e = 0; e < 2; ++e){
        deadLines[l] = {
            eyes[e].x + eyesRadius * cosf(angle45),
            eyes[e].y + eyesRadius * sinf(angle45),
            eyes[e].x + eyesRadius * cosf(angle225),
            eyes[e].y + eyesRadius * sinf(angle225)
        };
        ++l;

        deadLines[l] = {
            eyes[e].x + eyesRadius * cosf(angle135),
            eyes[e].y + eyesRadius * sinf(angle135),
            eyes[e].x + eyesRadius * cosf(angle315),
            eyes[e].y + eyesRadius * sinf(angle315)
        };
        ++l;
    }
}

void Head::RotatePupils(const Vector2 &pupilsFollowTarget){
    float angle;
    for (uint i = 0; i < 2; ++i){
        angle = atan2f(pupilsFollowTarget.y - pupils[i].y, pupilsFollowTarget.x - pupils[i].x);
        pupils[i] = {
            eyes[i].x + (eyesRadius - pupilsRadius) * cosf(angle),
            eyes[i].y + (eyesRadius - pupilsRadius) * sinf(angle)
        };
    }
}

void Head::Kill(){ isAlive = false; }
bool Head::IsAlive() const { return isAlive; }
Vector2 Head::GetCollisionPoint() const{ return collisionPoint; }

void Head::Update(const Vector2 &newPosition, const float newAngleOfMovement, const Vector2 &pupilsFollowTarget){
    position = newPosition;
    angleOfMovement = newAngleOfMovement;
    if (isAlive) collisionPoint = {
        position.x + eyesOffsetY * cosf(angleOfMovement),
        position.y + eyesOffsetY * sinf(angleOfMovement)
    };
    CalculateSymmetricOffsets(eyes, eyesOffsetX, eyesOffsetY); //New position for eyes
    CalculateSymmetricOffsets(nostrils, eyesOffsetY, -eyesOffsetY); //New position for nostrils
    if (isAlive) RotatePupils(pupilsFollowTarget);
    else UpdateDeadEyes();
}

void Head::Draw() const{
    //Head
    DrawCircleV(position, radius, color);

    //Eyes
    for (uint i = 0; i < 2; ++i){
        DrawCircleV(eyes[i], eyesBackRadius, color);
    }
        
    if (isAlive){
        for (uint i = 0; i < 2; ++i){
            DrawCircleV(eyes[i], eyesRadius, WHITE);
            DrawCircleV(pupils[i], pupilsRadius, BLACK);
        }
    }
    else{
        for (uint i = 0; i < 4; ++i){
            DrawLineEx({deadLines[i].x, deadLines[i].y}, {deadLines[i].z, deadLines[i].w}, eyesRadius / 2.0f, DARKGRAY);
        }
    }

    //Nostrils
    for (uint i = 0; i < 2; ++i){
        DrawCircleV(nostrils[i], nostrilsRadius, DARKGRAY);
    }
}