#include "Functions/functions.hpp"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Snake Remake");
    ResizeWindow(1.5, 1, 1);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    
    // ResourceManager::GetInstance().LoadF();
    ResourceManager::GetInstance().LoadAllTextures();
    ColorController::GetInstance().LoadColorsFrom("Resources/Colors.json");

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