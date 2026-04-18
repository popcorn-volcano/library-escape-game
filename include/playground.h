//
// Created by Liu KT on 2024/11/20.
//

#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <SDL.h>
#include <string>
#include <vector>
#include <memory>
#include "character.h"
#include "constants.h"  // Add this for PLAYGROUNDID
#include "tool.h"      // Add this for loadTexture
#include "obstacle.h"
#include "table.h"
#include "bookshelf.h"
#include "enemy.h" // Add this include
#include "game_state.h"  // For SCOREID
#include "game_management.h" // For GameManagement

// For collectibles
#include "collectible.h"  
#include "note.h"         
#include "coffee.h"       
#include "freeze.h"       
#include "pastexam.h"     
#include "score_page.h"  // Include the full header
#include "settings.h"    // Include the full header

// Forward declarations
class ScorePage;
class settings;
extern ScorePage* scorePage;
extern settings* SettingsPage;
extern int PAGE_ID;

class playground {
    friend class CollectibleManager;  // Allow CollectibleManager to access private members
private:
    static const int TOTAL_NOTES = 6;  // Change from 2 to 6
    static const int TOTAL_EXAMS = 2;  // Keep exam count at 2

    struct Button {
        SDL_Rect rect;
        const char* text;
        bool isHovered;
        SDL_Texture* textTexture;
    };

    struct PowerupIcon {
        SDL_Texture* texture;
        SDL_Rect position;
        bool isActive;
        float timeLeft;  // For displaying duration
    };

    SDL_Renderer* renderer;
    SDL_Texture* background;
    Character* mainCharacter; // Declare mainCharacter here
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<Collectible>> collectibles;  // Add this line
    std::vector<std::unique_ptr<Enemy>> enemies; // Declare enemies vector
    bool m_isPaused;  // Changed from isPaused to m_isPaused
    TTF_Font* font; // Font for pause menu
    std::vector<Button> pauseButtons;
    SDL_Color buttonColor;
    SDL_Color buttonHoverColor;
    bool isGameEnded;  // Add this to track game end state
    float collectTimer;
    bool isCollecting;

    // Add timer related members
    float gameTime = 60.0f;  // in seconds
    float currentTime;
    TTF_Font* timerFont;      // Separate font for timer
    SDL_Color timerColor;     // Color for timer text
    SDL_Rect timerRect;       // Position for timer display
    bool isTimerWarning;      // For visual effects when time is low

    // Add these members
    PowerupIcon coffeeIcon;
    PowerupIcon freezeIcon;
    SDL_Color iconBackgroundColor = {50, 50, 50, 200};  // Semi-transparent dark gray

    // Add these members for score display
    TTF_Font* scoreFont;
    SDL_Rect scoreRect;
    SDL_Color scoreColor;
    int currentScore;

    // Helper method for timer
    void renderTimer();
    void updateTimer(float deltaTime);
    std::string formatTime(float timeInSeconds);

    void createPauseButton(const char* text, int x, int y, int width, int height);
    void handleMouseMotion(int x, int y);
    int handleMouseClick(int x, int y);

    // Add helper method
    bool isAdjacentToCollectible(SDL_Rect playerPos, SDL_Rect collectiblePos) const;

    // Add helper methods
    void initializePowerupIcons();
    void renderPowerupIcons();
    void updatePowerupIcons(float deltaTime);

    // Add this helper method
    void renderScore();
    void updateScore();

    std::vector<std::unique_ptr<Collectible>> queuedCollectibles;  // Add this to store queued collectibles

    const char* scoreCharacterPaths[3] = {
        "../imgs/character1.png",
        "../imgs/character2.png", 
        "../imgs/character3.png"
    };

public:
    playground(const std::string& path, SDL_Renderer* renderer, const std::string& playerName);
    ~playground();
    
    // For collectibles
    void initializeCollectibles();
    void updateCollectibles(float deltaTime);
    void renderCollectibles();
    void checkCollectibles();  // Add new method
    
    // Existing methods
    void render();
    int update(float deltaTime);  // Modified signature
    int process_input(SDL_Event* event);
    bool isPositionBlocked(int x, int y) const;

    void addTable(int x, int y, int w, int h);
    void addBookshelf(int x, int y, int w, int h);
    void initializePauseMenu();
    void reset();    // Add method to reset/restart the game

    // Add accessor methods
    void setCoffeeIconState(bool active, float duration) {
        coffeeIcon.isActive = active;
        coffeeIcon.timeLeft = duration;
    }

    void setFreezeIconState(bool active, float duration) {
        freezeIcon.isActive = active;
        freezeIcon.timeLeft = duration;
    }

    Character* getMainCharacter() const { return mainCharacter; } // Add this method

    bool isEnemyCollision(int x, int y) const; // Add this method declaration

    bool isPaused() const { return m_isPaused; }  // Add this getter method
    void setPaused(bool paused) { m_isPaused = paused; }  // Add this setter method

    void updateSelectedCharacter() {
        if (SettingsPage) {
            // Get index of selected character
            int selectedIdx = SettingsPage->getSelectedCharacterIndex();
            
            // Load the corresponding score page texture
            SDL_Texture* scoreTexture = loadTexture(scoreCharacterPaths[selectedIdx], renderer);
            SDL_Rect charRect = SettingsPage->getSelectedCharacterRect();
            
            scorePage->setCharacterTexture(scoreTexture, charRect);
        }
    }

    void endGame(); // Just declare it here, don't define it

    void spawnNewNote();  // Add this declaration
    void spawnNewExam();  // Add this declaration
    void spawnQueuedCollectibles();  // Add this declaration

    void queueCollectible(std::unique_ptr<Collectible> collectible) {
        queuedCollectibles.push_back(std::move(collectible));
    }

    void reduceTimer(float seconds) { currentTime -= seconds; } // Add this method
};
#endif
