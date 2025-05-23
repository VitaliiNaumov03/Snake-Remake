#pragma once

#include "../Snake/snake.hpp"
#include "../Food/food.hpp"
#include "../ResourceManager/resource_manager.hpp"
#include "../ColorController/color_controller.hpp"
#include <memory>

std::shared_ptr<Snake> CreateSnake();
std::array<Food, 3> CreateFood();
void MainGame(std::shared_ptr<Snake> snake, Food *const food, Food *const bigFood, Food *const poison);
void SnakeDead(std::shared_ptr<Snake> snake, std::array<Food, 3> &food);