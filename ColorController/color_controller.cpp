#include "color_controller.hpp"

#define RANDOM_VALUE GetRandomValue(0, colorPairs.size() - 1)

ColorController &ColorController::GetInstance(){
    static ColorController instance;
    return instance;
}

void ColorController::LoadColorsFrom(const std::string jsonFileName){
    if (!FileExists(jsonFileName.c_str()))
        throw std::runtime_error("File not found: " + jsonFileName);
    
    std::ifstream jsonFile(jsonFileName);
    
    //Checking if file can be opened
    if (!jsonFile)
        throw std::runtime_error("Cannot open file: " + jsonFileName);
    
    nlohmann::json jsonReader;

    //Trying to parse file as json
    try{
        jsonFile >> jsonReader;
    }
    catch(const nlohmann::json::parse_error& e){
        throw std::runtime_error("JSON parse error in " + jsonFileName + ": " + e.what());
    }
    
    //Checking if "colors" array exists and is indeed an array
    if (!jsonReader.contains("colors") || !jsonReader["colors"].is_array())
        throw std::runtime_error("Invalid JSON structure: missing 'colors' array in " + jsonFileName);

    //Checking general structure of color pair [[r,g,b], [r,g,b]]
    for (const auto &arr : jsonReader["colors"]){
        if (!arr.is_array() || arr.size() != 2 ||
            !arr[0].is_array() || arr[0].size() != 3 ||
            !arr[1].is_array() || arr[1].size() != 3){
            continue; //Ignore invalid color pair
        }
        
        //Checking values in each color (integers from 0 to 255)
        bool colorIsValid = true;
        for (const auto &color : arr){
            for (const auto &val : color){
                if (!val.is_number_integer() || val < 0 || val > 255){
                    colorIsValid = false;
                    break;
                }
            }
            if (!colorIsValid) break;
        }
        if (!colorIsValid) continue; //Ignore this color pair and go to the next one

        //If everything is fine, save this color pair
        colorPairs.emplace_back(std::array<Color, 2>{{
            {arr[0][0], arr[0][1], arr[0][2], 255},
            {arr[1][0], arr[1][1], arr[1][2], 255}
        }});
    }
    if (colorPairs.size() < 1)
        throw std::runtime_error("No valid color pairs in " + jsonFileName);
    
    curr = RANDOM_VALUE;
}

void ColorController::UpdateSnakeLength(const uint newLength){
    colorCache.clear();
    colorCache.reserve(newLength);
    
    colorCache.emplace_back(colorPairs[curr][0]);
    
    Color newColor;
    for (uint i = 1; i < newLength - 1; ++i){
        newColor = ColorLerp(colorPairs[curr][0], colorPairs[curr][1], (float)i / newLength);
        colorCache.emplace_back(newColor);
    }

    colorCache.emplace_back(colorPairs[curr][1]);
}

void ColorController::SwitchToNextColor(){    
    if (colorPairs.size() == 1) return;
    
    uint next = RANDOM_VALUE;

    while (curr == next){
        next = RANDOM_VALUE;
    }

    curr = next;
}

const Color& ColorController::GetColorFor(const uint segmentIndex) const{
    return colorCache[segmentIndex];
}