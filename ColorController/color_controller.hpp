#pragma once

#include "../RayTools/raytools.hpp"
#include "../Libs/json.hpp"
#include <array>
#include <vector>
#include <stdexcept>
#include <fstream>

class ColorController{
private:
    uint curr;
    std::vector<std::array<Color, 2>> colorPairs;
    std::vector<Color> colorCache;
public:
    ColorController() = default;
    ~ColorController() = default;

    void LoadColorsFrom(const std::string jsonFileName);
    void UpdateSnakeLength(const uint newLength);
    void SwitchToNextColor();
    const Color& GetColorFor(const uint segmentIndex) const;
};