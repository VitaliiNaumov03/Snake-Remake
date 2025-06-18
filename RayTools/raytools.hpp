#pragma once

#include "../Libs/raylib.h"
#include "../Libs/raymath.h"

#define RAD_TO_DEG 57.2957795f
#define HALF_PI 1.57079632679489661923f

typedef unsigned int uint;

/**
 * @brief New window size = (monitorWidth / scalingIndex) or (monitorHeight / scalingIndex);
 * 
 * Monitor parameter depends on it's aspect ratio and is chosen automatically, so that window fits in;
 * 
 * aspectRatioX and aspectRatioY can either take "1920, 1080" or "16, 9" format.
 * 
 * @param aspectRatioX horizontal aspect ratio of a new window, for example, 16
 * @param aspectRatioY vertical aspect ratio of a new window, for example, 9
 * 
 */
void ResizeWindow(const float scalingIndex, const uint aspectRatioX, const uint aspectRatioY);

class Stopwatch{
private:
    Stopwatch(const Stopwatch&) = delete;
    Stopwatch &operator=(const Stopwatch&) = delete;

    float elapsedTime;
public:
    Stopwatch();
    Stopwatch(const float startTime);
    ~Stopwatch() = default;

    void Tick();
    void Reset();
    float GetElapsedTimeS() const;
    float GetElapsedTimeMs() const;
};

namespace Easings{
    float EaseInCubic(const float x);
    float EaseOutCubic(const float x);
    float EaseInOutCubic(const float x);
}