#include "head.hpp"

#define OFFSET radius / 2.0f
#define EYES_BACK_RADIUS roundf(radius / 1.5f)
#define EYES_RADIUS roundf(EYES_BACK_RADIUS / 2.0f)
#define PUPILS_RADIUS EYES_RADIUS / 1.6f

Head::Head(const Vector2 &startPosition, const uint radius, const float startAngleOfMovement, const Color &color) :
    isAlive(true),
    position(startPosition),
    radius(radius),
    angleOfMovement(startAngleOfMovement),
    color(color),
    eyesPosition(roundf(radius * 2.0f / 3.0f)){
    
    //CREATING ALIVE HEAD TEXTURE
    const Vector2 textureSize = {
        (float)radius * 2 + EYES_BACK_RADIUS - OFFSET,
        (float)(radius + EYES_BACK_RADIUS - (radius - eyesPosition)) * 2
    };

    headTexture = LoadRenderTexture((int)textureSize.x, (int)textureSize.y);
    
    //EYES
    const auto eyes = CalculateEyesPosition();
    RotatePupils({ 0.0f, 0.0f }); //Initializes base position
    RotatePupils({ //Rotates forward
        position.x + textureSize.x * 2 * cosf(angleOfMovement),
        position.y + textureSize.y * 2 * sinf(angleOfMovement)
    });
    
    //NOSTRILS
    const uint nostrilsRadius = roundf(radius * 0.083f);
    const auto nostrils = CalculateNostrilsPosition();

    SetTextureWrap(headTexture.texture, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(headTexture.texture, TEXTURE_FILTER_BILINEAR);
    BeginTextureMode(headTexture);
        DrawCircle(textureSize.x - radius, textureSize.y / 2.0f, radius, color); //Head circle
        for (uint i = 0; i < 2; ++i){
            DrawCircle(eyes[i].x, eyes[i].y, EYES_BACK_RADIUS, color);
            DrawCircle(eyes[i].x, eyes[i].y, EYES_RADIUS, WHITE);
            DrawCircle(nostrils[i].x, nostrils[i].y, nostrilsRadius, DARKGRAY);
        }
    EndTextureMode();
}

const std::array<Vector2, 2> Head::CalculateEyesPosition() const{
    return {{
        { //Left eye
            headTexture.texture.width - radius - OFFSET,
            headTexture.texture.height / 2.0f - eyesPosition
        },
        { //Right eye
            headTexture.texture.width - radius - OFFSET,
            headTexture.texture.height / 2.0f + eyesPosition
        }
    }};
}

const std::array<Vector2, 2> Head::CalculateNostrilsPosition() const{
    return {{
        { //Left nostril
            headTexture.texture.width - OFFSET,
            headTexture.texture.height / 2.0f - OFFSET
        },
        { //Right nostril
            headTexture.texture.width - OFFSET,
            headTexture.texture.height / 2.0f + OFFSET
        }
    }};
}

void Head::MakeDeadTexture(const std::array<Vector2, 2> eyes){
    const uint lineThickness = EYES_RADIUS / 2.0f;
    Vector4 line;

    BeginTextureMode(headTexture);
        for (uint i = 0; i < 2; ++i){
            DrawCircle(eyes[i].x, eyes[i].y, EYES_RADIUS, color);

            line = {
                eyes[i].x + EYES_RADIUS * cosf(3.92699f),
                eyes[i].y + EYES_RADIUS * sinf(3.92699f),
                eyes[i].x + EYES_RADIUS * cosf(0.785398f),
                eyes[i].y + EYES_RADIUS * sinf(0.785398f)
            };
            DrawLineEx({line.x, line.y}, {line.z, line.w}, lineThickness, DARKGRAY);
            
            line = {
                eyes[i].x + EYES_RADIUS * cosf(2.35619f),
                eyes[i].y + EYES_RADIUS * sinf(2.35619f),
                eyes[i].x + EYES_RADIUS * cosf(5.49779f),
                eyes[i].y + EYES_RADIUS * sinf(5.49779f)
            };
            DrawLineEx({line.x, line.y}, {line.z, line.w}, lineThickness, DARKGRAY);
        }
    EndTextureMode();
}

void Head::RotatePupils(const Vector2 &pupilsFollowTarget){
    const Vector2 offset = {
        radius / 2.0f * cosf(angleOfMovement),
        radius / 2.0f * sinf(angleOfMovement)
    };

    //Left pupil
    const float angleL = atan2(pupilsFollowTarget.y - pupils[0].y, pupilsFollowTarget.x - pupils[0].x);
    pupils[0] = {
        roundf(position.x - offset.x + eyesPosition * cosf(angleOfMovement - HALF_PI) + (EYES_RADIUS - PUPILS_RADIUS) * cosf(angleL)),
        roundf(position.y - offset.y + eyesPosition * sinf(angleOfMovement - HALF_PI) + (EYES_RADIUS - PUPILS_RADIUS) * sinf(angleL))
    };

    //Right pupil
    const float angleR = atan2(pupilsFollowTarget.y - pupils[1].y, pupilsFollowTarget.x - pupils[1].x);
    pupils[1] = {
        roundf(position.x - offset.x + eyesPosition * cosf(angleOfMovement + HALF_PI) + (EYES_RADIUS - PUPILS_RADIUS) * cosf(angleR)),
        roundf(position.y - offset.y + eyesPosition * sinf(angleOfMovement + HALF_PI) + (EYES_RADIUS - PUPILS_RADIUS) * sinf(angleR))
    };
}

void Head::Kill(){
    MakeDeadTexture(CalculateEyesPosition());
    isAlive = false;
}

bool Head::IsAlive() const { return isAlive; }

Vector2 Head::GetCollisionPoint() const{ return collisionPoint; }

void Head::Update(const Vector2 &newPosition, const float newAngle, const Vector2 &pupilsFollowTarget){
    position = newPosition;
    angleOfMovement = newAngle;
    collisionPoint = {
        position.x + OFFSET * cosf(angleOfMovement),
        position.y + OFFSET * sinf(angleOfMovement)
    };
    RotatePupils(pupilsFollowTarget);
}

void Head::Draw() const{
    //Head
    DrawTexturePro(headTexture.texture,
        {0.0f, 0.0f, (float)headTexture.texture.width, (float)-headTexture.texture.height},
        {position.x, position.y, (float)headTexture.texture.width, (float)headTexture.texture.height},
        {(float)headTexture.texture.width - radius, headTexture.texture.height / 2.0f},
        angleOfMovement * RAD_TO_DEG,
        WHITE
    );

    //Pupils
    if (isAlive){
        for (uint i = 0; i < 2; ++i)
            DrawCircle(pupils[i].x, pupils[i].y, PUPILS_RADIUS, BLACK);
    }
}