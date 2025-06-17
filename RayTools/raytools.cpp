#include "raytools.hpp"
#include <random>
#include <chrono>

void ResizeWindow(const float scalingIndex, const uint aspectRatioX, const uint aspectRatioY){
    const float aspectRatio = (float)aspectRatioY / aspectRatioX;
    const uint monitorWidth = GetMonitorWidth(GetCurrentMonitor());
    const uint monitorHeight = GetMonitorHeight(GetCurrentMonitor());

    uint newWidth = monitorWidth / scalingIndex;
    uint newHeight = monitorHeight / scalingIndex;
    newWidth += newWidth % 2;
    newHeight += newHeight % 2;

    if (monitorWidth > monitorHeight){
        if (newHeight / aspectRatio > monitorWidth)
            newHeight = newWidth * aspectRatio;
        else
            newWidth = newHeight / aspectRatio;
    }
    else{
        if (newWidth * aspectRatio > monitorHeight)
            newWidth = newHeight / aspectRatio;
        else
            newHeight = newWidth * aspectRatio;
    }
    
    SetWindowSize(newWidth, newHeight);
    SetWindowPosition((monitorWidth - newWidth) / 2, (monitorHeight - newHeight) / 2);
    TraceLog(LOG_INFO, "RAYTOOLS: ResizeWindow");
    TraceLog(LOG_INFO, "    > New width: %d", newWidth);
    TraceLog(LOG_INFO, "    > New height: %d", newHeight);
}

Stopwatch::Stopwatch() : elapsedTime(0.0f){};
Stopwatch::Stopwatch(const float startTime) : elapsedTime(startTime){};
void Stopwatch::Tick(){ elapsedTime += GetFrameTime(); }
void Stopwatch::Reset(){ elapsedTime = 0.0f; }
float Stopwatch::GetElapsedTimeS() const{ return elapsedTime; }
float Stopwatch::GetElapsedTimeMs() const{ return elapsedTime * 1000.0f; }

float Easings::EaseInCubic(const float x){
    return powf(x, 3);
}

float Easings::EaseOutCubic(const float x){
    return 1 - powf(1 - x, 3);
}

float Easings::EaseInOutCubic(const float x){
    return x < 0.5f ? 4.0f * powf(x, 3) : 1.0f - powf(-2.0f * x + 2.0f, 3.0f) / 2.0f;
}