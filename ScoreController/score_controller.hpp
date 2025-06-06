#pragma once

#include "../RayTools/raytools.hpp"
#include <cstdint>
#include <cstring>

class ScoreController{
private:
    ScoreController();
    ~ScoreController() = default;
    ScoreController(const ScoreController&) = delete;
    ScoreController &operator=(const ScoreController&) = delete;
    
    uint16_t currScore;
    uint16_t bestScore;
    bool bestScoreChanged;
public:
    static ScoreController &GetInstance();
    ScoreController &operator+=(const uint value);

    bool BestScoreChanged() const;
    void LoadBestScore();
    void WriteBestScore();
    void ResetCurrentScore();
    void UpdateTitle() const;
};