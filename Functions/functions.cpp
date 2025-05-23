#include "functions.hpp"

#define SNAKE_RADIUS (GetScreenWidth() * 0.03125f) //Radius of snake's body segments
// #define SNAKE_RADIUS 24 //For debug (testing different sizes)

std::shared_ptr<Snake> CreateSnake(){
    const uint windowSize = GetScreenWidth();
    const Vector2 position = {windowSize / 2.0f, windowSize / 2.0f};
    const uint startLength = 7;
    uint radius = SNAKE_RADIUS;
    radius += radius % 2;
    const uint segmentsGap = radius;
    const uint speed = roundf(windowSize / 3.0f);
    
    ColorController::GetInstance().UpdateSnakeLength(startLength);
    return std::make_shared<Snake>(position, startLength, radius, segmentsGap, speed);
}

std::array<std::unique_ptr<Food>, 3> CreateFood(){
    ResourceManager &resMan = ResourceManager::GetInstance();
    return {
        std::make_unique<Food>(&resMan.Get(ResourceManager::TextureID::Food), 1, SNAKE_RADIUS * 2),
        std::make_unique<BigFood>(&resMan.Get(ResourceManager::TextureID::BigFood), 2, SNAKE_RADIUS * 3),
        std::make_unique<Food>(&resMan.Get(ResourceManager::TextureID::Food), 1, SNAKE_RADIUS * 2)
    };
}

void GenerateNewFoodPosition(Food *const food, std::shared_ptr<Snake> snake){
    do {
        food->GenerateNewPosition();
    } while (snake->CollidesWith(food->GetPosition()));
}

void MainGame(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food){
    //Start position for food
    for (uint i = 0; i < food.size() - 1; ++i){
        GenerateNewFoodPosition(food[i].get(), snake);
    }
    
    while (!WindowShouldClose()){
        for (uint i = 0; i < food.size() - 1; ++i){
            food[i]->Update();
        }

        snake->Update(GetMousePosition(), food[1]->IsActive() ? food[1]->GetPosition() : food[0]->GetPosition());

        for (uint i = 0; i < food.size() - 1; ++i){
            if (food[i]->IsActive()){
                if (food[i]->WillSpawnNow()){
                    GenerateNewFoodPosition(food[i].get(), snake);
                }
                
                if (snake->Bites(food[i]->GetPosition(), food[i]->GetRadius())){
                    food[i]->Disappear();
                }
            }
        }

        if (snake->BitesItself()){
            snake->Kill(Snake::CauseOfDeath::BitItself);
            return;
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (uint i = 0; i < food.size() - 1; ++i)
                food[i]->Draw();
            snake->Draw();
            DrawFPS(0, 0);
        EndDrawing();
    }
}

void SnakeDead(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food){
    Stopwatch pause;

    while (!WindowShouldClose()){
        if (!snake->UpdateDead()){
            pause.Tick();
            //Frame at 1.0 second must be drawn too, that's why > instead of >=
            if (pause.GetElapsedTimeS() > 1.0f)
                return;
        }

        for (uint i = 0; i < food.size() - 1; ++i)
            food[i]->Update();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (uint i = 0; i < food.size() - 1; ++i)
                food[i]->Draw();
            snake->Draw();
        EndDrawing();
    }
}