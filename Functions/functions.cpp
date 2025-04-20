#include "functions.hpp"

void MainGame(const uint windowSize, ColorController *colorController){
    const Vector2 position = {windowSize / 2.0f, windowSize / 2.0f};
    const uint startLength = 7;
    uint radius = windowSize * 0.035f;
    // uint radius = 24;
    if (radius % 2 != 0) ++radius;
    const uint segmentsGap = radius;
    const uint speed = roundf(windowSize / 3.0f);
    
    colorController->UpdateSnakeLength(startLength);
    auto snake = std::make_unique<Snake>(position, startLength, radius, segmentsGap, speed, colorController);

    Vector2 destination;
    while (!WindowShouldClose()){
        if (snake->IsAlive() && snake->BitesItself()){
            snake->Kill(Snake::CauseOfDeath::BitItself);
        }

        if (snake->IsAlive()){
            destination = GetMousePosition();
            snake->Update(destination, destination);
        }
        else{
            snake->UpdateDead();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            snake->Draw();
            DrawFPS(0, 0);
        EndDrawing();
    }
}