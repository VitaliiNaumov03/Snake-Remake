#include "raytools.hpp"
#include <random>
#include <chrono>

Vector2 ResizeWindow(const float scalingIndex, const uint aspectRatioX, const uint aspectRatioY){
    const float aspectRatio = (float)aspectRatioY / aspectRatioX;
    const uint monitorWidth = GetMonitorWidth(GetCurrentMonitor());
    const uint monitorHeight = GetMonitorHeight(GetCurrentMonitor());

    float newWidth = monitorWidth / scalingIndex;
    float newHeight = monitorHeight / scalingIndex;

    if (newWidth * aspectRatio > newHeight)
        newWidth = newHeight / aspectRatio;
    else
        newHeight = newWidth * aspectRatio;
    
    SetWindowSize(newWidth, newHeight);
    SetWindowPosition((monitorWidth - newWidth) / 2, (monitorHeight - newHeight) / 2);

    return (Vector2){newWidth, newHeight};
}

Stopwatch::Stopwatch() : elapsedTime(0.0f){};
Stopwatch::Stopwatch(const float startTime) : elapsedTime(startTime){};
void Stopwatch::Tick(){ elapsedTime += GetFrameTime(); }
void Stopwatch::Reset(){ elapsedTime = 0.0f; }
float Stopwatch::GetElapsedTimeS() const{ return elapsedTime; }
float Stopwatch::GetElapsedTimeMs() const{ return elapsedTime * 1000.0f; }

float Easings::EaseOutBack(const float t){
    const float c1 = 1.70158f;
    const float c2 = c1 + 1.0f;
    return 1.0f + c2 * powf(t - 1.0f, 3) + c1 * powf(t - 1.0f, 2);
}