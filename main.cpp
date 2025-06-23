#ifndef __APPLE__
#include "Functions/icon32.h"
#endif
#include "Functions/functions.hpp"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Snake Remake");
    ResizeWindow(1.5f, 1, 1);
    #ifndef __APPLE__
    SetIcon();
    #endif
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    
    std::future<void> imageLoader = std::async(std::launch::async,
        &ResourceManager::PreloadGameImages,
        &ResourceManager::GetInstance());

    std::future<void> colorsLoader = std::async(std::launch::async,
        &ColorController::LoadColorsFrom,
        &ColorController::GetInstance(),
        "Resources/Colors.json");

    try{
        Intro({&imageLoader, &colorsLoader});
        imageLoader.get();
        colorsLoader.get();
    }
    catch(const std::exception &e){
        CloseWindow();
        ShowError(e.what());
        return -1;
    }

    ResourceManager::GetInstance().LoadTexturesFromImages(TEXTURE_FILTER_BILINEAR);
    ScoreController::GetInstance().LoadBestScore();

    std::shared_ptr<Snake> snake;
    auto food = CreateFood();

    while(!WindowShouldClose()){
        snake = CreateSnake();
        FadeIn(ColorController::GetInstance().GetColorFor(0));
        ZoomOut(snake);
        MainGame(snake, food);
        SnakeDead(snake, food);
        FadeOut(snake, food);
        for (auto &food : food){
            food->Reset();
        }
        if (ScoreController::GetInstance().BestScoreChanged()){
            ScoreController::GetInstance().WriteBestScore();
        }
        if (!WindowShouldClose()){
            ScoreController::GetInstance().ResetCurrentScore();
            ColorController::GetInstance().SwitchToNextColor();
        }
    }
    CloseWindow();
    return 0;
}