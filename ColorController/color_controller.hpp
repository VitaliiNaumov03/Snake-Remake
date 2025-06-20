#pragma once

#include "../RayTools/raytools.hpp"
#include "../Libs/json.hpp"
#include <array>
#include <vector>
#include <stdexcept>
#include <fstream>

class ColorController{
private:
    ColorController() = default;
    ~ColorController() = default;
    ColorController(const ColorController&) = delete;
    ColorController &operator=(const ColorController&) = delete;

    uint curr;
    std::vector<std::array<Color, 2>> colorPairs;
    std::vector<Color> colorCache;
    Color cursorColor;

    bool ColorIsValid(const nlohmann::json &arr) const;
    void LoadValidColorPairsFrom(const nlohmann::json &arr);
public:
    static ColorController &GetInstance();
    void LoadColorsFrom(const std::string jsonFileName);
    void UpdateSnakeLength(const uint newLength);
    void SwitchToNextColor();
    Color GetCursorColor() const;
    Color GetColorFor(const uint segmentIndex) const;
};