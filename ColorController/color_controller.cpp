#include "color_controller.hpp"

#define RANDOM_VALUE GetRandomValue(0, colorPairs.size() - 1)

ColorController &ColorController::GetInstance(){
    static ColorController instance;
    return instance;
}

//Checks each value in color (integers from 0 to 255)
bool ColorController::ColorIsValid(const nlohmann::json &color) const{
    if (color.size() != 3 || !color.is_array()) return false;
    for (const auto &value : color){
        if (!value.is_number_integer() || value < 0 || value > 255)
            return false;
    }
    return true;
}

void ColorController::LoadValidColorPairsFrom(const nlohmann::json &array){
    if (array.size() == 0) return;

    //Checking general structure of color pair [[r,g,b], [r,g,b]]
    for (const auto &colorPair : array){
        if (!colorPair.is_array() || colorPair.size() != 2 ||
            !ColorIsValid(colorPair[0]) || !ColorIsValid(colorPair[1]))
            continue; //Skip invalid color pair
        
        //If everything is fine, save this color pair
        colorPairs.emplace_back(std::array<Color, 2>{{
            {colorPair[0][0], colorPair[0][1], colorPair[0][2], 255},
            {colorPair[1][0], colorPair[1][1], colorPair[1][2], 255}
        }});
    }
}

void ColorController::LoadColorsFrom(const std::string jsonFileName){
    if (!FileExists(jsonFileName.c_str())){
        throw std::runtime_error("File not found: \"" + jsonFileName + "\"");
    }
    
    std::ifstream jsonFile(jsonFileName);
    
    if (!jsonFile){
        throw std::runtime_error("Cannot open file: \"" + jsonFileName + "\"");
    }
    
    nlohmann::json jsonReader;

    try{
        jsonFile >> jsonReader;
    }
    catch(const nlohmann::json::parse_error& e){
        throw std::runtime_error("JSON parse error in \"" + jsonFileName + "\": " + e.what());
    }
    
    //Checking if "cursor" array exists
    if (!jsonReader.contains("cursor")){
        throw std::runtime_error("Invalid JSON structure: missing \"cursor\" array in \"" + jsonFileName + "\"");
    }

    if (ColorIsValid(jsonReader["cursor"])){
        cursorColor = {
            jsonReader["cursor"][0],
            jsonReader["cursor"][1],
            jsonReader["cursor"][2],
            255
        };
    }
    else{
        throw std::runtime_error("Invalid \"cursor\" array in \"" + jsonFileName + "\"");
    }

    //Checking if "body" array exists and is indeed an array
    if (!jsonReader.contains("body")){
        throw std::runtime_error("Invalid JSON structure: missing \"body\" array in \"" + jsonFileName + "\"");
    }
    
    if (!jsonReader["body"].is_array()){
        throw std::runtime_error("Invalid JSON structure: \"body\" is not an array in \"" + jsonFileName + "\"");
    }

    LoadValidColorPairsFrom(jsonReader["body"]);

    if (colorPairs.size() < 1){
        throw std::runtime_error("No valid color pairs in \"" + jsonFileName + "\"");
    }
    
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

Color ColorController::GetCursorColor() const{ return cursorColor; }
Color ColorController::GetColorFor(const uint segmentIndex) const{ return colorCache[segmentIndex]; }