#include "score_controller.hpp"

ScoreController::ScoreController() :
    currScore(0),
    bestScore(0){}

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

    int bytesRead = 0;
    unsigned char* data = LoadFileData(SAVE_FILE_PATH, &bytesRead);

    if (bytesRead != sizeof(uint)){
        UnloadFileData(data);
        return;
    }

    std::memcpy(&bestScore, data, sizeof(uint)); //Restore uint from bytes
    UnloadFileData(data);
}

void ScoreController::WriteBestScore() const{
    unsigned char data[sizeof(uint)];
    std::memcpy(data, &bestScore, sizeof(uint));
    SaveFileData(SAVE_FILE_PATH, data, sizeof(uint));
}

void ScoreController::ResetCurrentScore(){ currScore = 0; }

void ScoreController::UpdateTitle() const{
    SetWindowTitle(TextFormat("🍎: %d 🏆: %d", currScore, bestScore));
}

void ScoreController::HideScore() const { SetWindowTitle("Snake Remake"); }