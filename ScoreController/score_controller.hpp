#pragma once

#include "../RayTools/raytools.hpp"
#include <cstring>

#define SAVE_FILE_PATH "Resources/BestScore.snk"

class ScoreController{
private:
    ScoreController();
    ~ScoreController() = default;
    ScoreController(const ScoreController&) = delete;
    ScoreController &operator=(const ScoreController&) = delete;
    
    uint currScore;
    uint bestScore;
public:
    static ScoreController &GetInstance();
    ScoreController &operator+=(const uint value);

    void LoadBestScore();
    void WriteBestScore() const;
    void ResetCurrentScore();
    void UpdateTitle() const;
    void HideScore() const;
};