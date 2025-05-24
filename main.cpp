#include "Functions/functions.hpp"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Snake Remake");
    ResizeWindow(1.5, 1, 1);
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    HideCursor();
    
    ResourceManager::GetInstance().LoadAllTextures();
    ColorController::GetInstance().LoadColorsFrom("Resources/colors.json");

    std::shared_ptr<Snake> snake;
    std::array<std::unique_ptr<Food>, 3> food = CreateFood();

    while(!WindowShouldClose()){
        snake = CreateSnake();
        MainGame(snake, food);
        SnakeDead(snake, food);
        for (auto &food : food)
            food->Reset();
        ColorController::GetInstance().SwitchToNextColor();
    }
    
    CloseWindow();
    return 0;
}