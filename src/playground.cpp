//
// Created by Liu KT on 2024/11/20.
//

#include "../include/playground.h"
#include "../include/character.h"
#include "../include/tool.h"
#include "../include/constants.h"

playground::playground(const std::string& backgroundPath, SDL_Renderer* renderer, const std::string& playerName)
    : renderer(renderer), isPaused(false)
{
    SDL_Log("Initializing playground...");

    // Load background texture
    background = loadTexture(backgroundPath.c_str(), renderer);
    if (!background) {
        SDL_Log("Failed to load playground background!");
        // Handle error appropriately
    } else {
        SDL_Log("Playground background loaded.");
    }

    // Initialize character
    mainCharacter = new Character(renderer, playerName, this, 5, 5);
    if (!mainCharacter) {
        SDL_Log("Failed to create main character!");
        // Handle error appropriately
    } else {
        SDL_Log("Main character created.");
    }

    // Initialize obstacles
    // Center table
    obstacles.push_back(std::make_unique<Table>(renderer, 10, 10, 2, 2));  
    SDL_Log("Center table added.");

    // Add more tables around the playground
    // Study area tables
    obstacles.push_back(std::make_unique<Table>(renderer, 5, 5, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 5, 8, 2, 2));
    SDL_Log("Study area tables added.");

    // Group work tables
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 5, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 8, 2, 2));
    SDL_Log("Group work tables added.");

    // Reading area tables
    obstacles.push_back(std::make_unique<Table>(renderer, 15, 15, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 15, 2, 2));
    SDL_Log("Reading area tables added.");

    // Keep existing bookshelf
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 15, 12, 2, 3));
    SDL_Log("Bookshelf obstacle added.");

    initializePauseMenu();
    SDL_Log("Playground initialized.");
}

void playground::initializePauseMenu() {
    font = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 36);
    if (!font) {
        SDL_Log("Failed to load font for pause menu: %s", TTF_GetError());
        return;
    }

    buttonColor = {100, 100, 100, 255};
    buttonHoverColor = {150, 150, 150, 255};

    int scale = 2;
    int baseWidth = 150 * scale;
    int baseHeight = 35 * scale;
    
    // Calculate center positions based on window dimensions (675 * 2, 386 * 2)
    int screenWidth = 675 * 2;
    int screenHeight = 386 * 2;
    
    // Center horizontally by subtracting half the button width from half the screen width
    int centerX = (screenWidth - baseWidth) / 2;
    
    // Center vertically and space buttons apart
    int startY = (screenHeight - (baseHeight * 2 + 20)) / 2;  // 20 pixels spacing between buttons
    
    createPauseButton("Resume", centerX, startY, baseWidth, baseHeight);
    createPauseButton("Surrender", centerX, startY + baseHeight + 20, baseWidth, baseHeight);
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
    if (isPaused) {
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
                    isPaused = false;  // Resume game if ESC is pressed again
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
                    case SDLK_ESCAPE:
                        isPaused = true;  // Enter pause state
                        break;
                }
                break;
        }
    }
    return PLAYGROUNDID;
}

void playground::handleMouseMotion(int x, int y) {
    if (!isPaused) return;
    
    for (auto& button : pauseButtons) {
        button.isHovered = (x >= button.rect.x && x <= button.rect.x + button.rect.w &&
                           y >= button.rect.y && y <= button.rect.y + button.rect.h);
    }
}

int playground::handleMouseClick(int x, int y) {
    if (!isPaused) return PLAYGROUNDID;

    for (size_t i = 0; i < pauseButtons.size(); i++) {
        if (x >= pauseButtons[i].rect.x && x <= pauseButtons[i].rect.x + pauseButtons[i].rect.w &&
            y >= pauseButtons[i].rect.y && y <= pauseButtons[i].rect.y + pauseButtons[i].rect.h) {
            
            switch(i) {
                case 0:  // Resume
                    isPaused = false;
                    return PLAYGROUNDID;
                case 1:  // Surrender
                    endGame();  // Call endGame before returning to menu
                    return MENUID;
            }
        }
    }
    return PLAYGROUNDID;
}

int playground::update() {
    return TRUE;
}

void playground::render() {
    SDL_RenderClear(renderer);

    // Render game
    if (background) {
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
    }

    // Render obstacles
    for (const auto& obstacle : obstacles) {
        obstacle->render();
    }

    // Render main character
    mainCharacter->render();

    // Render pause menu if paused
    if (isPaused) {
        // Dim background
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
        SDL_Rect fullscreen = {0, 0, 675 * 2, 386 * 2};
        SDL_RenderFillRect(renderer, &fullscreen);

        // Render pause buttons
        for (const auto& button : pauseButtons) {
            SDL_SetRenderDrawColor(renderer,
                button.isHovered ? buttonHoverColor.r : buttonColor.r,
                button.isHovered ? buttonHoverColor.g : buttonColor.g,
                button.isHovered ? buttonHoverColor.b : buttonColor.b,
                255);
            SDL_RenderFillRect(renderer, &button.rect);

            if (button.textTexture) {
                int textWidth, textHeight;
                SDL_QueryTexture(button.textTexture, nullptr, nullptr, &textWidth, &textHeight);
                SDL_Rect textRect = {
                    button.rect.x + (button.rect.w - textWidth) / 2,
                    button.rect.y + (button.rect.h - textHeight) / 2,
                    textWidth,
                    textHeight
                };
                SDL_RenderCopy(renderer, button.textTexture, nullptr, &textRect);
            }
        }
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
    // You can add score calculation here or call another class's method
    // For example: ScoreManager::getInstance()->calculateFinalScore();
}

void playground::reset() {
    // Reset game state
    isPaused = false;
    isGameEnded = false;
    
    // Reset character position
    if (mainCharacter) {
        delete mainCharacter;
    }
    mainCharacter = new Character(renderer, "Player1", this, 5, 5);
    
    // Clear and reinitialize obstacles
    obstacles.clear();
    // Center table
    obstacles.push_back(std::make_unique<Table>(renderer, 10, 10, 2, 2));  
    SDL_Log("Center table added.");

    // Add more tables around the playground
    // Study area tables
    obstacles.push_back(std::make_unique<Table>(renderer, 5, 5, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 5, 8, 2, 2));
    SDL_Log("Study area tables added.");

    // Group work tables
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 5, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 8, 2, 2));
    SDL_Log("Group work tables added.");

    // Reading area tables
    obstacles.push_back(std::make_unique<Table>(renderer, 15, 15, 2, 2));
    obstacles.push_back(std::make_unique<Table>(renderer, 20, 15, 2, 2));
    SDL_Log("Reading area tables added.");

    // Keep existing bookshelf
    obstacles.push_back(std::make_unique<Bookshelf>(renderer, 15, 12, 2, 3));
    SDL_Log("Bookshelf obstacle added.");
    
    // Reset any other game-specific variables
}

playground::~playground() {
    if (background) {
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    // Delete the mainCharacter
    delete mainCharacter;
}
