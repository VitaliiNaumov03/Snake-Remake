#include "functions.hpp"

#define SNAKE_RADIUS (GetScreenWidth() * 0.03125f) //Radius of snake's body segments
// #define SNAKE_RADIUS 48 //For debug (testing different sizes)

void GenerateIcon(const uint size){
    auto canvas = LoadRenderTexture(size, size);
    const Color &headColor = ColorController::GetInstance().GetColorFor(0);
    const Vector2 position = {size / 2.0f, size / 2.7f};
    
    Head head(position, position.y, 3 * HALF_PI, headColor);
    
    BeginTextureMode(canvas);
        ClearBackground(Color{ headColor.r, headColor.g, headColor.b, 0 });
        DrawCircleV({position.x, position.y * 3.0f}, position.y, ColorController::GetInstance().GetColorFor(2));
        DrawCircleV({position.x, position.y * 2.0f}, position.y, ColorController::GetInstance().GetColorFor(1));
        head.Draw();
    EndTextureMode();
    SetTextureWrap(canvas.texture, TEXTURE_WRAP_CLAMP);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);

    Image icon = LoadImageFromTexture(canvas.texture);
    ImageFlipVertical(&icon);
    SetWindowIcon(icon);
    UnloadRenderTexture(canvas);
    UnloadImage(icon);
}

std::shared_ptr<Snake> CreateSnake(){
    const uint windowSize = GetScreenWidth();
    const Vector2 position = {windowSize / 2.0f, windowSize / 2.0f};
    const uint startLength = 4; //NOTE: GenerateIcon() needs at least 3 segments at start
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
        std::make_unique<Poison>(&resMan.Get(ResourceManager::TextureID::Poison), 0, SNAKE_RADIUS * 2, 30.0f)
    };
}

void GenerateNewFoodPosition(Food *const food, std::shared_ptr<Snake> snake){
    do {
        food->GenerateNewPosition();
    } while (snake->BodyCollidesWith(food->GetPosition()));
}

void MainGame(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food){
    HideCursor();
    ScoreController::GetInstance().UpdateTitle();
    Vector2 mousePosition;
    const uint cursorRadius = snake->GetRadius() / 2.0f;
    const Texture2D &backgroundTop = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundTop);
    const Texture2D &backgroundBottom = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundBottom);

    GenerateNewFoodPosition(food[0].get(), snake); //Start position for regular food
    
    while (!WindowShouldClose()){
        mousePosition = GetMousePosition();
        for (uint i = 0; i < food.size(); ++i)
            food[i]->Update();
        snake->Update(mousePosition, cursorRadius, food[1]->IsActive() ? food[1]->GetPosition() : food[0]->GetPosition());

        //Food spawning & eating logic
        for (uint i = 0; i < food.size(); ++i){
            if (food[i]->IsActive()){
                if (food[i]->WillSpawnNow()){
                    GenerateNewFoodPosition(food[i].get(), snake);
                }
                
                if (snake->Bites(food[i]->GetPosition(), food[i]->GetRadius())){
                    if (i == 2){
                        food[2]->Disappear();
                        snake->Kill(Snake::CauseOfDeath::AtePoison);
                        return;
                    }
                    const uint points = food[i]->Eat();
                    snake->Grow(points);
                    ScoreController::GetInstance() += points;
                    ColorController::GetInstance().UpdateSnakeLength(snake->GetLength());
                }
            }
        }

        if (snake->BitesItself()){
            snake->Kill(Snake::CauseOfDeath::BitItself);
            return;
        }
        
        BeginDrawing();
            DrawTexturePro(backgroundBottom,
                {0.0f, 0.0f, (float)backgroundBottom.width, (float)backgroundBottom.height},
                {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                {0.0f, 0.0f},
                0.0f,
                WHITE
            );
            for (uint i = 0; i < food.size(); ++i)
            food[i]->Draw();
            snake->Draw();
            DrawTexturePro(backgroundTop,
                {0.0f, 0.0f, (float)backgroundTop.width, (float)backgroundTop.height},
                {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                {0.0f, 0.0f},
                0.0f,
                WHITE
            );
            DrawCircleV(mousePosition, cursorRadius, MAROON); //Cursor
        EndDrawing();
    }
}

void SnakeDead(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food){
    ShowCursor();
    Stopwatch pause;
    const Texture2D &backgroundTop = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundTop);
    const Texture2D &backgroundBottom = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundBottom);

    while (!WindowShouldClose()){
        if (!snake->UpdateDead()){
            pause.Tick();
            if (pause.GetElapsedTimeS() >= 1.0f)
                return;
        }

        for (uint i = 0; i < food.size(); ++i)
            food[i]->Update();

        BeginDrawing();
            DrawTexturePro(backgroundBottom,
                {0.0f, 0.0f, (float)backgroundBottom.width, (float)backgroundBottom.height},
                {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                {0.0f, 0.0f},
                0.0f,
                WHITE
            );
            for (uint i = 0; i < food.size(); ++i)
                food[i]->Draw();
            snake->Draw();
            DrawTexturePro(backgroundTop,
                {0.0f, 0.0f, (float)backgroundTop.width, (float)backgroundTop.height},
                {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                {0.0f, 0.0f},
                0.0f,
                WHITE
            );
        EndDrawing();
    }
}