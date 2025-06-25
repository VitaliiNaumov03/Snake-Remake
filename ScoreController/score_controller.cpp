#include "score_controller.hpp"
#include <filesystem>

#define SAVE_FILE_NAME "BestScore.snk"

ScoreController::ScoreController() : currScore(0), bestScore(0), bestScoreChanged(false){}

ScoreController &ScoreController::GetInstance(){
    static ScoreController instance;
    return instance;
}

ScoreController &ScoreController::operator+=(const uint value){
    currScore += value;
    if (currScore > bestScore){
        bestScore = currScore;
        bestScoreChanged = true;
    }
    UpdateTitle();
    return *this;
}

bool ScoreController::BestScoreChanged() const{ return bestScoreChanged; }

//Returns full save file path based on current OS
std::string ScoreController::GetSaveFilePath() const{
#ifdef _WIN32
    const char* userProfile = std::getenv("USERPROFILE");
    std::filesystem::path saveDir = std::filesystem::path(userProfile) / "Documents" / "My Games" / "Snake Remake";
#elif __APPLE__
    const char* home = std::getenv("HOME");
    std::filesystem::path saveDir = std::filesystem::path(home) / "Library" / "Application Support" / "Snake Remake";
#else
    const char* home = std::getenv("HOME");
    std::filesystem::path saveDir = std::filesystem::path(home) / ".local" / "share" / "Snake Remake";
#endif

    std::filesystem::create_directories(saveDir);
    return (saveDir / SAVE_FILE_NAME).string();
}

void ScoreController::LoadBestScore(){
    const auto saveFilePath = GetSaveFilePath();
    if (!FileExists(saveFilePath.c_str())) return;

    int dataSize = 0;
    unsigned char* data = LoadFileData(saveFilePath.c_str(), &dataSize);

    if (data && dataSize == sizeof(uint16_t)) {
        bestScore = *reinterpret_cast<uint16_t*>(data);
        UnloadFileData(data);
    }
}

void ScoreController::WriteBestScore(){
    SaveFileData(GetSaveFilePath().c_str(), &bestScore, sizeof(bestScore));
}

void ScoreController::ResetCurrentScore(){ 
    currScore = 0;
    UpdateTitle();
}

void ScoreController::UpdateTitle() const{
    SetWindowTitle(TextFormat("🍎: %d 🏆: %d", currScore, bestScore));
}