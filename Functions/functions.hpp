#pragma once

#include "../Snake/snake.hpp"
#include "../Food/food.hpp"
#include "../ResourceManager/resource_manager.hpp"
#include "../ColorController/color_controller.hpp"
#include "../ScoreController/score_controller.hpp"
#include <memory>
#include <future>

#ifdef _WIN32
    extern "C" __declspec(dllimport)
    int __stdcall MessageBoxA(void* hWnd, const char* lpText, const char* lpCaption, unsigned int uType);
#endif

std::shared_ptr<Snake> CreateSnake();
std::array<std::unique_ptr<Food>, 3> CreateFood();
void GenerateNewFoodPosition(Food *const food, std::shared_ptr<Snake> snake);
bool AllLoadersFinished(std::vector<std::future<void>*> &loadersToTrack);
float CalculateStartZoom();

void Intro(std::vector<std::future<void>*> loadersToTrack);
void FadeIn(const Color color);
void ZoomOut(std::shared_ptr<Snake> snake);
void MainGame(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food);
void SnakeDead(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food);
void FadeOut(std::shared_ptr<Snake> snake, std::array<std::unique_ptr<Food>, 3> &food);

void ShowError(const std::string &message);