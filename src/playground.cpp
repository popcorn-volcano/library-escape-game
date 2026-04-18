//
// Created by Liu KT on 2024/11/20.
//

#include "../include/playground.h"
#include "../include/game_state.h"
#include "../include/game_management.h"
#include "../include/collectible_manager.h"
#include "../include/character.h"
#include "../include/tool.h"
#include "../include/constants.h"
#include "../include/freeze.h"
#include "../include/game_state.h"
#include "../include/score_page.h" // Add this include
#include "../include/enemy.h" // Add this include
#include <set>
playground::playground(const std::string& backgroundPath, SDL_Renderer* renderer, const std::string& playerName)
    : renderer(renderer), m_isPaused(false), collectTimer(0), isCollecting(false)
{
    // Set this playground instance as current
    CollectibleManager::setCurrentPlayground(this);
    
    SDL_Log("Initializing playground...");
    
    // Initialize game management system
    GameManagement::initialize();

    // Load background texture
    background = loadTexture(backgroundPath.c_str(), renderer);
    if (!background) {
        SDL_Log("Failed to load playground background!");
        // Handle error appropriatel
    } else {
        SDL_Log("Playground background loaded.");
    }

    // Initialize character
    mainCharacter = new Character(renderer, playerName, this, 16, 9);
    if (!mainCharacter) {
        SDL_Log("Failed to creat main character!");
        // Handle error appropriately
    } else {
        SDL_Log("Main character created.");
    }

    // Initialize obstacles
    // Center table
    obstacles.push_back(std::make_unique<Table>(renderer, 10, 10, 2, 2));  
    SDL_Log("Center table added.");
   
    // Initialize test coffee collectible
    collectibles.push_back(std::make_unique<Coffee>(renderer, 10, 10));  // Place at grid position 10,10
    
    initializePauseMenu();
    
    // In playground initialization
    GameManagement::initialize();

    initializeCollectibles();  // Initialize collectibles

    // Initialize timer
    currentTime = gameTime;
    timerFont = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 128);  // Larger font for timer
    timerColor = {255, 255, 255, 255};  // White color
    timerRect = {20, 20, 150, 50};  // Position in top-left corner
    isTimerWarning = false;

    // Initialize powerup icons
    initializePowerupIcons();

    // Set icon background color
    iconBackgroundColor = {50, 50, 50, 200};  // Semi-transparent dark gray

    // Initialize score display
    scoreFont = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 48);
    scoreColor = {255, 215, 0, 255};  // Gold color
    scoreRect = {1100, 20, 200, 50};  // Position in top-right corner
    currentScore = 0;

    // Initialize enemies
    std::vector<std::pair<int, int>> waypoints = {{5, 5}, {10, 5}, {10, 10}, {5, 10}};
    std::vector<std::pair<int, int>> enemyPositions = {{5, 5}, {15, 15}, {25, 5}, {5, 15}, {25, 15}};
    
    for (const auto& pos : enemyPositions) {
        if (pos.first != 16 || pos.second != 9) { // Ensure no overlap with main character
            enemies.push_back(std::make_unique<Enemy>(renderer, "Enemy", this, pos.first, pos.second, waypoints, 1));
        }
    }
}

void playground::initializePauseMenu() {
    font = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 128);
    if (!font) {
        SDL_Log("Failed to load font for pause menu: %s", TTF_GetError());
        return;
    }

    // Enhanced button colors
    buttonColor = {80, 80, 80, 255};          // Darker gray for better contrast
    buttonHoverColor = {120, 120, 120, 255};  // Lighter gray for hover state

    // Calculate center positions based on window dimensions
    int screenWidth = 1350;   // 675 * 2
    int screenHeight = 772;   // 386 * 2
    
    // Slightly larger button dimensions
    int buttonWidth = 400;    // Increased from 300
    int buttonHeight = 100;   // Increased from 80
    int buttonSpacing = 50;   // Increased from 40
    
    // Center horizontally and move up vertically
    int centerX = (screenWidth - buttonWidth) / 2;
    int startY = (screenHeight - (buttonHeight * 2 + buttonSpacing)) / 2 - 50;
    
    createPauseButton("Resume", centerX, startY, buttonWidth, buttonHeight);
    createPauseButton("Surrender", centerX, startY + buttonHeight + buttonSpacing, buttonWidth, buttonHeight);
}

void playground::createPauseButton(const char* text, int x, int y, int width, int height) {
    Button button = {};
    button.rect = {x, y, width, height};
    button.text = text;
    button.isHovered = false;
    
    if (font) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text, {255, 255, 255, 255});
        if (surface) {
            button.textTexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
    }
    
    pauseButtons.push_back(button);
}

int playground::process_input(SDL_Event* event) {
    if (isPaused()) {
        switch (event->type) {
            case SDL_MOUSEMOTION:
                handleMouseMotion(event->motion.x, event->motion.y);
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event->button.button == SDL_BUTTON_LEFT) {
                    return handleMouseClick(event->button.x, event->button.y);
                }
                break;
            case SDL_KEYDOWN:
                if (event->key.keysym.sym == SDLK_ESCAPE) {
                    setPaused(false);  // Resume game if ESC is pressed again
                }
                break;
        }
        return PLAYGROUNDID;
    } else {
        switch (event->type) {
            case SDL_KEYDOWN:
                switch (event->key.keysym.sym) {
                    case SDLK_w:
                        mainCharacter->moveUp();
                        break;
                    case SDLK_s:
                        mainCharacter->moveDown();
                        break;
                    case SDLK_a:
                        mainCharacter->moveLeft();
                        break;
                    case SDLK_d:
                        mainCharacter->moveRight();
                        break;
                    case SDLK_e:  // Start collecting when E is pressed
                        isCollecting = true;
                        SDL_Log("Started collecting attempt");  // Add debug log
                        break;
                    case SDLK_ESCAPE:
                        setPaused(true);  // Enter pause state
                        break;
                }
                break;
            case SDL_KEYUP:
                if (event->key.keysym.sym == SDLK_e) {
                    isCollecting = false;
                    collectTimer = 0;
                    SDL_Log("Stopped collecting attempt");  // Add debug log
                }
                break;
        }
    }
    return PLAYGROUNDID;
}

void playground::handleMouseMotion(int x, int y) {
    if (!isPaused()) return;
    
    for (auto& button : pauseButtons) {
        button.isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                           y >= button.rect.y && y <= button.rect.y + button.rect.h);
    }
}

int playground::handleMouseClick(int x, int y) {
    if (!isPaused()) return PLAYGROUNDID;

    for (size_t i = 0; i < pauseButtons.size(); i++) {
        if (x >= pauseButtons[i].rect.x && x <= pauseButtons[i].rect.x + pauseButtons[i].rect.w &&
            y >= pauseButtons[i].rect.y && y <= pauseButtons[i].rect.y + pauseButtons[i].rect.h) {
            
            switch(i) {
                case 0:  // Resume
                    setPaused(false);
                    return PLAYGROUNDID;
                case 1:  // Surrender
                    endGame();  // Call endGame before returning
                    return SCOREID;  // Return SCOREID instead of MENUID to show score page
            }
        }
    }
    return PLAYGROUNDID;
}

int playground::update(float deltaTime) {
    // Update collectibles
    for (auto& collectible : collectibles) {
        collectible->update(deltaTime);
    }

    // Add debug logging to verify timing
    for (const auto& collectible : collectibles) {
        if (auto coffee = dynamic_cast<Coffee*>(collectible.get())) {
            if (coffee->isBoostActive()) {
                SDL_Log("Coffee power-up time remaining: %.2f", coffee->getBoostDuration());
            }
        }
    }
    
    // Check for collection
    if (isCollecting) {
        checkCollectibles();
    }
    
    updateTimer(deltaTime);

    // Update enemies
    for (auto& enemy : enemies) {
        enemy->update();
    }

    return TRUE;
}

void playground::render() {
    SDL_RenderClear(renderer);
    
    // Render background
    if (background) {
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }
    
    // Render game elements
    for (const auto& obstacle : obstacles) {
        obstacle->render();
    }
    renderCollectibles();
    mainCharacter->render();
    
    // Render timer
    renderTimer();
    
    // Render powerup icons
    renderPowerupIcons();
    
    // Render pause menu if paused
    if (isPaused()) {
        // Add semi-transparent overlay
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect fullscreen = {0, 0, 1350, 772};
        SDL_RenderFillRect(renderer, &fullscreen);
        
        // Render pause buttons
        for (const auto& button : pauseButtons) {
            // Render button background
            SDL_SetRenderDrawColor(renderer,
                button.isHovered ? buttonHoverColor.r : buttonColor.r,
                button.isHovered ? buttonHoverColor.g : buttonColor.g,
                button.isHovered ? buttonHoverColor.b : buttonColor.b,
                255);
            SDL_RenderFillRect(renderer, &button.rect);
            
            // Render button text
            if (button.textTexture) {
                SDL_RenderCopy(renderer, button.textTexture, nullptr, &button.rect);
            }
        }
    }
    
    // Add score rendering
    renderScore();

    // Render enemies
    for (const auto& enemy : enemies) {
        enemy->render();
    }
    
    SDL_RenderPresent(renderer);
}

void playground::addTable(int x, int y, int w, int h) {
    obstacles.push_back(std::make_unique<Table>(renderer, x, y, w, h));
}

void playground::addBookshelf(int x, int y, int w, int h) {
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, x, y, w, h));
}

bool playground::isPositionBlocked(int x, int y) const {
    for (const auto& obstacle : obstacles) {
        if (obstacle->isBlocking(x, y)) {
            return true;
        }
    }
    return false;
}

void playground::endGame() {
    isGameEnded = true;
    GameManagement::printStats();
    updateSelectedCharacter();
    PAGE_ID = SCOREID;
    scorePage->initialize();
}

void playground::reset() {
    setPaused(false);  // This line is correct
    isGameEnded = false;
    
    // Reset timer
    currentTime = gameTime;  // Reset to 60 seconds (defined in playground.h)
    isTimerWarning = false;

    // Rest of the reset function remains the same...
    if (mainCharacter) {
        delete mainCharacter;
    }
    mainCharacter = new Character(renderer, "Player1", this, 16, 9);
    
    obstacles.clear();
    obstacles.push_back(std::make_unique<Table>(renderer, 10, 10, 2, 2));
    // Group work tables
    obstacles.push_back(std::make_unique<Table>(renderer, 1, 0, 2, 2, TABLE_TEXTURE_1x1bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 28, 0, 2, 2, TABLE_TEXTURE_2x2book));
    obstacles.push_back(std::make_unique<Table>(renderer, 30, 0, 2, 2, TABLE_TEXTURE_1x1book));

    obstacles.push_back(std::make_unique<Table>(renderer, 12, 4, 2, 2, TABLE_TEXTURE_2x2bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 22, 4, 2, 2, TABLE_TEXTURE_2x2book));
    obstacles.push_back(std::make_unique<Table>(renderer, 30, 5, 2, 2, TABLE_TEXTURE_1x1book));
    obstacles.push_back(std::make_unique<Table>(renderer, 2, 5, 2, 2, TABLE_TEXTURE_1x1bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 27, 5, 2, 2, TABLE_TEXTURE_1x1bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 17, 2, 2, 2, TABLE_TEXTURE_1x1book));
    
    //obstacles.push_back(std::make_unique<Table>(renderer, 22, 6, 2, 2, TABLE_TEXTURE_2x2book));
    //obstacles.push_back(std::make_unique<Table>(renderer, 17, 7, 2, 2, TABLE_TEXTURE_1x1book));
    //obstacles.push_back(std::make_unique<Table>(renderer, 12, 9, 2, 2, TABLE_TEXTURE_2x2bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 26, 10, 2, 2, TABLE_TEXTURE_2x2book));
    //obstacles.push_back(std::make_unique<Table>(renderer, 8, 14, 2, 2, TABLE_TEXTURE_1x1bag));

    obstacles.push_back(std::make_unique<Table>(renderer, 27,15, 2, 2, TABLE_TEXTURE_2x2bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 22, 16, 2, 2, TABLE_TEXTURE_1x1book));
    obstacles.push_back(std::make_unique<Table>(renderer, 17, 15, 2, 2, TABLE_TEXTURE_2x2book));
    obstacles.push_back(std::make_unique<Table>(renderer, 12, 16, 2, 2, TABLE_TEXTURE_1x1bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 2,16, 2, 2, TABLE_TEXTURE_2x2bag));
    obstacles.push_back(std::make_unique<Table>(renderer, 7, 16, 2, 2, TABLE_TEXTURE_1x1book));
    //obstacles.push_back(std::make_unique<Table>(renderer, 4, 15, 2, 2, TABLE_TEXTURE_1x1book));
    obstacles.push_back(std::make_unique<Table>(renderer, 3, 10, 2, 2, TABLE_TEXTURE_1x1bag));
   
    // Keep existing bookshelf
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 27, 8, 1,2 , BOOKSHELF_TEXTURE_2X1));
   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 15, 7, 1, 3, BOOKSHELF_TEXTURE_3X1));
   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 12, 10, 1, 4, BOOKSHELF_TEXTURE_4X1));
   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 22, 9, 2, 4, BOOKSHELF_TEXTURE_4X2));

   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 0, 0, 1, 18, BOOKSHELF_TEXTURE_LEFT));
   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 3, 0, 25, 2, BOOKSHELF_TEXTURE_UP));
   obstacles.push_back(std::make_unique<Bookshelf>(renderer, 32, 0, 1, 18, BOOKSHELF_TEXTURE_RIGHT));
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 8, 4, 2, 4, BOOKSHELF_TEXTURE_4X2));
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 31, 10, 1,2 , BOOKSHELF_TEXTURE_2X1));
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 17, 9, 2, 4, BOOKSHELF_TEXTURE_4X2));

    // Reset collectibles
    collectibles.clear();
    initializeCollectibles();

    // Reset collection state
    isCollecting = false;
    collectTimer = 0;
    
    // Reset powerup icons
    coffeeIcon.isActive = false;
    freezeIcon.isActive = false;
}
//collectibles test
// Define a custom comparator for SDL_Point

void playground::initializeCollectibles() {
    std::set<SDL_Point, SDLPointCompare> usedSpawnPoints;
    
    for (int i = 0; i < TOTAL_NOTES; i++) {
        SDL_Point spawnPoint;
        do {
            spawnPoint = CollectibleManager::getRandomSpawnPoint();
        } while (usedSpawnPoints.count(spawnPoint) > 0);
        
        usedSpawnPoints.insert(spawnPoint);
        collectibles.push_back(std::make_unique<Note>(renderer, spawnPoint.x, spawnPoint.y));
    }
    
    // Then spawn exams
    for (int i = 0; i < TOTAL_EXAMS; i++) {
        SDL_Point spawnPoint;
        do {
            spawnPoint = CollectibleManager::getRandomSpawnPoint();
        } while (usedSpawnPoints.count(spawnPoint) > 0);
        
        usedSpawnPoints.insert(spawnPoint);
        collectibles.push_back(std::make_unique<PastExam>(renderer, spawnPoint.x, spawnPoint.y));
    }
    
    // Finally spawn power-ups
    SDL_Point coffeeSpawn;
    do {
        coffeeSpawn = CollectibleManager::getRandomSpawnPoint();
    } while (usedSpawnPoints.count(coffeeSpawn) > 0);
    usedSpawnPoints.insert(coffeeSpawn);
    
    SDL_Point freezeSpawn;
    do {
        freezeSpawn = CollectibleManager::getRandomSpawnPoint();
    } while (usedSpawnPoints.count(freezeSpawn) > 0);
    usedSpawnPoints.insert(freezeSpawn);
    
    collectibles.push_back(std::make_unique<Coffee>(renderer, coffeeSpawn.x, coffeeSpawn.y));
    collectibles.push_back(std::make_unique<Freeze>(renderer, freezeSpawn.x, freezeSpawn.y));
}

void playground::updateCollectibles(float deltaTime) {
    // First process any pending spawns
    spawnQueuedCollectibles();
    
    // Then update existing collectibles
    for (auto& collectible : collectibles) {
        collectible->update(deltaTime);
    }
}

void playground::renderCollectibles() {
    for (auto& collectible : collectibles) {
        collectible->render();
    }
}
//collectibles test end

bool playground::isAdjacentToCollectible(SDL_Rect playerPos, SDL_Rect collectiblePos) const {
    // Convert pixel positions to grid positions
    int gridPlayerX = playerPos.x / GRID_SIZE;
    int gridPlayerY = playerPos.y / GRID_SIZE;
    int gridCollectibleX = collectiblePos.x / GRID_SIZE;
    int gridCollectibleY = collectiblePos.y / GRID_SIZE;
    
    SDL_Log("Grid comparison - Player: (%d,%d), Collectible: (%d,%d)", 
            gridPlayerX, gridPlayerY, gridCollectibleX, gridCollectibleY);
    
    bool isLeftOrRight = (abs(gridPlayerX - gridCollectibleX) == 1) && 
                        (gridPlayerY == gridCollectibleY);
    bool isAboveOrBelow = (abs(gridPlayerY - gridCollectibleY) == 1) && 
                         (gridPlayerX == gridCollectibleX);
    
    return isLeftOrRight || isAboveOrBelow;
}

void playground::checkCollectibles() {
    SDL_Rect playerPos = mainCharacter->getPosition();
    
    bool foundAdjacentCollectible = false;
    for (auto& collectible : collectibles) {
        if (collectible->isVisible()) {
            SDL_Rect collectiblePos = collectible->getPosition();
            bool isAdjacent = isAdjacentToCollectible(playerPos, collectiblePos);
            
            if (isAdjacent) {
                foundAdjacentCollectible = true;
                if (isCollecting) {
                    if (!collectible->getIsBeingCollected()) {
                        collectible->startCollection();  // Start fresh collection
                    }
                    collectible->updateCollection(1.0f/60.0f);
                } else {
                    collectible->cancelCollection();  // Reset progress when E is released
                }
            } else {
                collectible->cancelCollection();  // Reset progress when moving away
            }
        }
    }
    
    // No need for the global reset since we handle each collectible individually
}

void playground::updateTimer(float deltaTime) {
    if (!isPaused() && !isGameEnded) {
        currentTime -= deltaTime;
        
        // Check for timer warning state (less than 30 seconds)
        isTimerWarning = currentTime <= 30.0f;
        
        // End game when timer reaches 0
        if (currentTime <= 0) {
            currentTime = 0;
            endGame();
        }
    }
}

void playground::renderTimer() {
    // Format time as MM:SS
    std::string timeStr = formatTime(currentTime);
    
    // Create timer texture
    SDL_Color textColor = timerColor;
    if (isTimerWarning) {
        // Pulse red color when time is low
        Uint8 pulse = static_cast<Uint8>(127 + 128 * sin(SDL_GetTicks() * 0.005f));
        textColor = {255, 0, 0, pulse};
    }
    
    SDL_Surface* surface = TTF_RenderText_Solid(timerFont, timeStr.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_RenderCopy(renderer, texture, nullptr, &timerRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

std::string playground::formatTime(float timeInSeconds) {
    int minutes = static_cast<int>(timeInSeconds) / 60;
    int seconds = static_cast<int>(timeInSeconds) % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02d:%02d", minutes, seconds);
    return std::string(buffer);
}

void playground::initializePowerupIcons() {
    // Load icons
    coffeeIcon.texture = loadTexture("../imgs/coffee.png", renderer);
    freezeIcon.texture = loadTexture("../imgs/freeze.png", renderer);
    
    // Position icons on the right side of the screen
    int iconSize = 48;  // Smaller than collectibles
    int margin = 20;
    int startY = 100;  // Below timer
    
    coffeeIcon.position = {1350 - margin - iconSize, startY, iconSize, iconSize};
    freezeIcon.position = {1350 - margin - iconSize, startY + iconSize + margin, iconSize, iconSize};
    
    coffeeIcon.isActive = false;
    freezeIcon.isActive = false;
}

void playground::renderPowerupIcons() {
    if (coffeeIcon.isActive || freezeIcon.isActive) {
        // Create timer font if not created yet
        TTF_Font* iconFont = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 24);  // Smaller font for icons
        
        if (coffeeIcon.isActive) {
            // Render background
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 
                iconBackgroundColor.r,
                iconBackgroundColor.g, 
                iconBackgroundColor.b,
                iconBackgroundColor.a);
            SDL_RenderFillRect(renderer, &coffeeIcon.position);
            
            // Render icon
            SDL_RenderCopy(renderer, coffeeIcon.texture, nullptr, &coffeeIcon.position);
            
            // Render countdown timer
            char timeBuffer[8];
            snprintf(timeBuffer, sizeof(timeBuffer), "%.1fs", coffeeIcon.timeLeft);
            SDL_Color timerColor = {255, 255, 255, 255};  // White text
            SDL_Surface* timeSurface = TTF_RenderText_Solid(iconFont, timeBuffer, timerColor);
            SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(renderer, timeSurface);
            
            // Position timer text to the left of the icon
            SDL_Rect timeRect = {
                coffeeIcon.position.x - 60,  // Left of icon
                coffeeIcon.position.y + 12,  // Vertically centered
                50,                          // Width for time text
                24                           // Height for time text
            };
            SDL_RenderCopy(renderer, timeTexture, nullptr, &timeRect);
            
            // Cleanup
            SDL_FreeSurface(timeSurface);
            SDL_DestroyTexture(timeTexture);
        }
        
        // Similar for freeze icon...
        if (freezeIcon.isActive) {
            // Existing freeze icon rendering...
        }
        
        TTF_CloseFont(iconFont);
    }
}

void playground::renderScore() {
    // Update score calculation to match new values
    currentScore = (GameManagement::getNoteCount() * 8) + 
                  (GameManagement::getExamCount() * 12);
    
    // Rest of the function remains the same...
    std::string scoreText = "Score: " + std::to_string(currentScore);
    SDL_Surface* surface = TTF_RenderText_Solid(scoreFont, scoreText.c_str(), scoreColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    // Add a semi-transparent background
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
    SDL_RenderFillRect(renderer, &scoreRect);
    
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

bool playground::isEnemyCollision(int x, int y) const {
    for (const auto& enemy : enemies) {
        if (enemy->getX() == x && enemy->getY() == y) {
            return true;
        }
    }
    return false;
}

playground::~playground() {
    if (background) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    // Delete the mainCharacter
    delete mainCharacter;

    // Existing cleanup...
    if (timerFont) {
        TTF_CloseFont(timerFont);
        timerFont = nullptr;
    }
    if (scoreFont) {
        TTF_CloseFont(scoreFont);
        scoreFont = nullptr;
    }
}

void playground::spawnNewNote() {
    std::set<SDL_Point, SDLPointCompare> usedPoints;
    
    // Get currently used positions
    for (const auto& collectible : collectibles) {
        if (collectible->isVisible()) {
            SDL_Rect pos = collectible->getPosition();
            usedPoints.insert({pos.x / GRID_SIZE, pos.y / GRID_SIZE});
        }
    }
    
    // Get a new spawn point that's not in use
    SDL_Point spawnPoint;
    do {
        spawnPoint = CollectibleManager::getRandomSpawnPoint();
    } while (usedPoints.count(spawnPoint) > 0);
    
    // Spawn new note
    collectibles.push_back(std::make_unique<Note>(renderer, spawnPoint.x, spawnPoint.y));
}

void playground::spawnNewExam() {
    std::set<SDL_Point, SDLPointCompare> usedPoints;
    
    // Get currently used positions
    for (const auto& collectible : collectibles) {
        if (collectible->isVisible()) {
            SDL_Rect pos = collectible->getPosition();
            usedPoints.insert({pos.x / GRID_SIZE, pos.y / GRID_SIZE});
        }
    }
    
    // Get a new spawn point that's not in use
    SDL_Point spawnPoint;
    do {
        spawnPoint = CollectibleManager::getRandomSpawnPoint();
    } while (usedPoints.count(spawnPoint) > 0);
    
    // Spawn new exam
    collectibles.push_back(std::make_unique<PastExam>(renderer, spawnPoint.x, spawnPoint.y));
}

void playground::spawnQueuedCollectibles() {
    // Move any queued collectibles into the main collection
    for (auto& collectible : queuedCollectibles) {
        collectibles.push_back(std::move(collectible));
    }
    queuedCollectibles.clear();
}
