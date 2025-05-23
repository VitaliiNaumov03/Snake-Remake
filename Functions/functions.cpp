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

std::array<Food, 3> CreateFood(){
    ResourceManager &resMan = ResourceManager::GetInstance();
    return {
        Food(&resMan.Get(ResourceManager::TextureID::Food), 1, SNAKE_RADIUS * 2),
        Food(&resMan.Get(ResourceManager::TextureID::Food), 1, SNAKE_RADIUS * 2),
        Food(&resMan.Get(ResourceManager::TextureID::Food), 1, SNAKE_RADIUS * 2)
    };
}

void GenerateNewFoodPosition(Food *const food, std::shared_ptr<Snake> snake){
    do {
        food->GenerateNewPosition();
    } while (snake->CollidesWith(food->GetPosition()));
}

void MainGame(std::shared_ptr<Snake> snake, Food *const food, Food *const bigFood, Food *const poison){
    GenerateNewFoodPosition(food, snake); //Start position for food
    
    while (!WindowShouldClose()){
        food->Update();
        snake->Update(GetMousePosition(), food->GetPosition());

        if (food->IsActive()){
            if (food->WillSpawnNow()){
                GenerateNewFoodPosition(food, snake);
            }
            
            if (snake->Bites(food->GetPosition(), food->GetRadius())){
                food->Disappear();
            }
        }

        if (snake->BitesItself()){
            snake->Kill(Snake::CauseOfDeath::BitItself);
            return;
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            food->Draw();
            snake->Draw();
            DrawFPS(0, 0);
        EndDrawing();
    }
}

void SnakeDead(std::shared_ptr<Snake> snake, std::array<Food, 3> &food){
    Stopwatch pause;

    while (!WindowShouldClose()){
        if (!snake->UpdateDead()){
            pause.Tick();
            //Frame at 1.0 second must be drawn too, that's why > instead of >=
            if (pause.GetElapsedTimeS() > 1.0f)
                return;
        }

        for (Food &food : food)
            food.Update();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            for (Food &food : food)
                food.Draw();
            snake->Draw();
        EndDrawing();
    }
}