#pragma once

#include "../RayTools/raytools.hpp"
#include "head.hpp"
#include "tongue.hpp"
#include "../ColorController/color_controller.hpp"
#include <vector>
#include <optional>

class Snake{
public:
    enum class CauseOfDeath {BitItself, AtePoison};

    Snake() = default;
    Snake(const Vector2 position, const uint startLength, const uint radius, const uint segmentsGap, const uint speed);
    ~Snake() = default;

    uint GetRadius() const;
    uint GetLength() const;
    bool IsAlive() const;
    bool Bites(const Vector2 &center, const float radius) const;
    bool BitesItself() const;
    bool BodyCollidesWith(const Vector2 &point) const;
    void Grow(const uint numOfNewSegments);
    void Kill(const CauseOfDeath causeOfDeath);
    void Update(const Vector2 &destination, const uint targetDistance, const Vector2 &pupilsFollowTarget);
    bool UpdateDead(); //Returns false when death animation is finished
    void Draw() const;
private:
    //Parameters
    uint radius;
    uint segmentsGap;
    uint speed;
    float angleOfMovement;
    int currShakeIntensity;
    Stopwatch shakeInterval; //Measures time for one shake (to make it FPS-independent)
    Stopwatch shakeAnimation; //Measures total time of shaking animation
    Vector2 deadPoint;
    CauseOfDeath causeOfDeath;
    ColorController *colContr;

    //Body parts
    Head head;
    static std::optional<Tongue> tongue;
    std::vector<Vector2> body;

    void RotateAndMove(Vector2 &origin, const Vector2 &destination, const uint targetDistance);
    void RotateAndMoveHead(Vector2 destination, uint targetDistance, const float targetSpeed);
};