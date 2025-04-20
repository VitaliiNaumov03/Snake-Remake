#include "Functions/functions.hpp"

#define COLORS_JSON_PATH "Resources/colors.json"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(0, 0, "Snake Remake");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    const uint windowSize = ResizeWindow(1.6, 1, 1).x;
    
    ColorController colorController; //SwitchToNextColor() вручную в функции (Snake не контролит)
    colorController.LoadColorsFrom(COLORS_JSON_PATH);
    
    MainGame(windowSize, &colorController);
    
    CloseWindow();
    return 0;
}