#include "functions.hpp"

#define SNAKE_RADIUS (GetScreenWidth() * 0.03125f) //Radius of snake's body segments
// #define SNAKE_RADIUS 48 //For debug (testing different sizes)

using namespace std::chrono_literals;

void SetIcon(){
    Image icon = {ICON32_DATA, ICON32_WIDTH, ICON32_HEIGHT, 0, ICON32_FORMAT};
    SetWindowIcon(icon);
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

void Intro(std::future<void> &loaderToTrack){
    ResourceManager::GetInstance().LoadTextureFromFile(ResourceManager::TextureID::Logo, "Resources/logo.png", TEXTURE_FILTER_TRILINEAR);
    const Texture2D &logo = ResourceManager::GetInstance().Get(ResourceManager::TextureID::Logo);
    Stopwatch pause;
    Stopwatch fadeStopwatch;

    enum State { FADE_IN, HOLD, FADE_OUT, DONE };
    State currState = FADE_IN;

    const float size = GetScreenWidth() * 0.35f;
    constexpr float fadeDurationS = 1.0f;
    constexpr float holdDurationS = 1.0f;

    float alpha = 0.0f;
    bool userWantsToSkip = false;

    while (!WindowShouldClose()){
        if (userWantsToSkip){
            if (loaderToTrack.wait_for(100ms) == std::future_status::ready) break;
        }
        else
            userWantsToSkip = IsKeyPressed(KEY_ENTER);
        
        switch (currState){
            case FADE_IN:
                fadeStopwatch.Tick();
                alpha = Easings::EaseInCubic(Clamp(fadeStopwatch.GetElapsedTimeS() / fadeDurationS, 0.0f, 1.0f));
                if (fadeStopwatch.GetElapsedTimeS() >= fadeDurationS){
                    alpha = 1.0f;
                    currState = HOLD;
                    fadeStopwatch.Reset();
                }
                break;

            case HOLD:
                pause.Tick();
                if (pause.GetElapsedTimeS() >= holdDurationS && loaderToTrack.wait_for(100ms) == std::future_status::ready){
                    currState = FADE_OUT;
                    fadeStopwatch.Reset();
                }
                break;

            case FADE_OUT:
                fadeStopwatch.Tick();
                alpha = 1.0f - Easings::EaseOutCubic(Clamp(fadeStopwatch.GetElapsedTimeS() / fadeDurationS, 0.0f, 1.0f));
                if (fadeStopwatch.GetElapsedTimeS() >= fadeDurationS){
                    currState = DONE;
                }
                break;

            case DONE:
                goto exit; //Exit while loop
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                logo,
                {0.0f, 0.0f, (float)logo.width, (float)logo.height},
                {(GetScreenWidth() - size) / 2.0f, (GetScreenHeight() - size) / 2.0f, size, size},
                {0.0f, 0.0f},
                0.0f,
                Fade(WHITE, alpha)
            );
        EndDrawing();
    }

    exit:
        ResourceManager::GetInstance().UnloadT(ResourceManager::TextureID::Logo);
}

void ZoomOut(std::shared_ptr<Snake> snake){
    Camera2D camera = { 0 };
    camera.target = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.offset = camera.target;
    camera.rotation = 0.0f;
    camera.zoom = 0.0f;

    Stopwatch zoomStopwatch;
    const Texture2D &backgroundBottom = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundBottom);
    const Texture2D &backgroundTop = ResourceManager::GetInstance().Get(ResourceManager::TextureID::BackgroundTop);
    constexpr float durationS = 2.5f;
    constexpr float startZoom = 50.0f;
    constexpr float endZoom = 1.0f;

    while (!WindowShouldClose()){
        zoomStopwatch.Tick();
        camera.zoom = startZoom + (endZoom - startZoom) * Easings::EaseInOutCubic(Clamp(zoomStopwatch.GetElapsedTimeS() / durationS, 0.0f, 1.0f));
        camera.zoom += GetMouseWheelMove() / 10.0f;
        // if (zoomStopwatch.GetElapsedTimeS() >= 1.0f){
        //     zoomStopwatch.Reset();
        //     TraceLog(LOG_INFO, "Current zoom: %f", camera.zoom);
        // }
    
        BeginDrawing();
        BeginMode2D(camera);
                DrawTexturePro(backgroundBottom,
                    {0.0f, 0.0f, (float)backgroundBottom.width, (float)backgroundBottom.height},
                    {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                    {0.0f, 0.0f},
                    0.0f,
                    WHITE
                );
                snake->Draw();
                DrawTexturePro(backgroundTop,
                    {0.0f, 0.0f, (float)backgroundTop.width, (float)backgroundTop.height},
                    {0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight()},
                    {0.0f, 0.0f},
                    0.0f,
                    WHITE
                );
        EndMode2D();
        EndDrawing();
    }
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

void ShowError(const std::string &message){
    #ifdef _WIN32
        MessageBoxA(nullptr, message.c_str(), "Error", 0x00000010L | 0x00000000L);
    #elif __APPLE__
        std::string escaped = message;
        size_t pos = 0;
        while ((pos = escaped.find("\"", pos)) != std::string::npos)
            escaped.replace(pos, 1, "\\\"");

        const std::string cmd = "osascript -e 'display dialog \"" + escaped +
                        "\" buttons {\"OK\"} with title \"Error\" with icon stop'";
        system(cmd.c_str());
    #endif
}