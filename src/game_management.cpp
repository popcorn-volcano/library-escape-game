#include "../include/game_management.h"

// Initialize static members
int GameManagement::noteCount = 0;
int GameManagement::examCount = 0;
int GameManagement::coffeeCount = 0;
int GameManagement::freezeCount = 0;
bool GameManagement::initialized = false;
bool GameManagement::isCoffeePowerupActive = false;

void GameManagement::initialize() {
    if (!initialized) {
        resetCounts();
        initialized = true;
        SDL_Log("Game Management System Initialized");
    }
}

void GameManagement::incrementCount(const std::string& type) {
    if (type == "note") {
        // Double points during coffee power-up
        if (isCoffeePowerupActive) {
            noteCount += 2;
        } else {
            noteCount++;
        }
        SDL_Log("Note collected! Total notes: %d", noteCount);
    }
    else if (type == "exam") {
        // Double points during coffee power-up
        if (isCoffeePowerupActive) {
            examCount += 2;
        } else {
            examCount++;
        }
        SDL_Log("Past Exam collected! Total exams: %d", examCount);
    }
    else if (type == "coffee") {
        coffeeCount++;
        SDL_Log("Coffee power-up collected! Total coffees: %d", coffeeCount);
    }
    else if (type == "freeze") {
        freezeCount++;
        SDL_Log("Freeze power-up collected! Total freezes: %d", freezeCount);
    }
}

void GameManagement::resetCounts() {
    noteCount = 0;
    examCount = 0;
    coffeeCount = 0;
    freezeCount = 0;
    SDL_Log("All collectible counts reset to 0");
}

void GameManagement::printStats() {
    SDL_Log("----- Collectibles Statistics -----");
    SDL_Log("Notes collected: %d", noteCount);
    SDL_Log("Past Exams collected: %d", examCount);
    SDL_Log("Coffee power-ups collected: %d", coffeeCount);
    SDL_Log("Freeze power-ups collected: %d", freezeCount);
    SDL_Log("Total collectibles: %d", noteCount + examCount + coffeeCount + freezeCount);
    SDL_Log("--------------------------------");
}