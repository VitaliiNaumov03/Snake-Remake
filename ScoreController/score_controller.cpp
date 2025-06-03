#include "score_controller.hpp"

ScoreController::ScoreController() : currScore(0), bestScore(0){}

ScoreController &ScoreController::GetInstance(){
    static ScoreController instance;
    return instance;
}

ScoreController &ScoreController::operator+=(const uint value){
    currScore += value;
    if (currScore > bestScore)
        bestScore = currScore;
    UpdateTitle();
    return *this;
}

void ScoreController::LoadBestScore(){
    if (!FileExists(SAVE_FILE_PATH)) return;

    int dataSize = 0;
    unsigned char* data = LoadFileData(SAVE_FILE_PATH, &dataSize);

    if (data && dataSize == sizeof(uint16_t)) {
        bestScore = *reinterpret_cast<uint16_t*>(data);
        UnloadFileData(data);
    }
}

void ScoreController::WriteBestScore(){
    SaveFileData(SAVE_FILE_PATH, &bestScore, sizeof(bestScore));
}

void ScoreController::ResetCurrentScore(){ currScore = 0; }

void ScoreController::UpdateTitle() const{
    SetWindowTitle(TextFormat("🍎: %d 🏆: %d", currScore, bestScore));
}