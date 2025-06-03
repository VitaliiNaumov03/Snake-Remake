#include "Functions/functions.hpp"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Snake Remake");
    ResizeWindow(1.5, 1, 1);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    
    try{
        Intro();
        ResourceManager::GetInstance().LoadGameTextures();
        ColorController::GetInstance().LoadColorsFrom("Resources/Colors.json");
    }
    catch(const std::exception &e){
        CloseWindow();
        ShowError(e.what());
        return -1;
    }

    std::shared_ptr<Snake> snake;
    std::array<std::unique_ptr<Food>, 3> food = CreateFood();

    while(!WindowShouldClose()){
        snake = CreateSnake();
        GenerateIcon(48);
        MainGame(snake, food);
        SnakeDead(snake, food);
        for (auto &food : food)
        food->Reset();
        ScoreController::GetInstance().ResetCurrentScore();
        ColorController::GetInstance().SwitchToNextColor();
    }
    CloseWindow();
    return 0;
}