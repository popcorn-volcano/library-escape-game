#include "../include/character.h"
#include "../include/tool.h"
#include "../include/playground.h"  // Add this include

Character::Character(SDL_Renderer* renderer, const std::string& name, 
                    const playground* playground, int startX, int startY)
    : renderer(renderer), name(name), gamePlayground(playground),
      gridX(startX), gridY(startY), facingRight(true),
      sprite(nullptr), nameFont(nullptr), nameTexture(nullptr), // Initialize all pointers
      skinColor{255, 200, 150, 255},  // Default skin color
      clothesColor{50, 50, 200, 255}, // Default clothes color
      hairColor{100, 50, 0, 255},     // Default hair color
      nameColor{255, 255, 255, 255}   // Default name color
{
    SDL_Log("Initializing character...");

    // Initialize font for name tag
    nameFont = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 16);
    if (!nameFont) {
        SDL_Log("Failed to load name font: %s", TTF_GetError());
        return;  // Early return on failure
    }
    SDL_Log("Name font loaded.");

    // Create name texture
    updateNameTexture();
    if (!nameTexture) {
        SDL_Log("Failed to create name texture!");
        return;
    }
    SDL_Log("Name texture created.");

    SDL_Log("Character initialized successfully.");
}

Character::~Character() {
    if (sprite) {
        SDL_DestroyTexture(sprite);
    }
    if (nameTexture) {
        SDL_DestroyTexture(nameTexture);
    }
    if (nameFont) {
        TTF_CloseFont(nameFont);
    }
}

void Character::moveLeft() {
    if (gridX > 0 && !gamePlayground->isPositionBlocked(gridX - 1, gridY)) {
        gridX--;
        facingRight = false;
    }
}

void Character::moveRight() {
    if (gridX < 31 && !gamePlayground->isPositionBlocked(gridX + 1, gridY)) {
        gridX++;
        facingRight = true;
    }
}

void Character::moveUp() {
    if (gridY > 0 && !gamePlayground->isPositionBlocked(gridX, gridY - 1)) {
        gridY--;
    }
}

void Character::moveDown() {
    if (gridY < 17 && !gamePlayground->isPositionBlocked(gridX, gridY + 1)) {
        gridY++;
    }
}

void Character::setSkinColor(Uint8 r, Uint8 g, Uint8 b) {
    skinColor = {r, g, b, 255};
}

void Character::setClothesColor(Uint8 r, Uint8 g, Uint8 b) {
    clothesColor = {r, g, b, 255};
}

void Character::setHairColor(Uint8 r, Uint8 g, Uint8 b) {
    hairColor = {r, g, b, 255};
}

void Character::setHairStyle(int style) {
    hairStyle = style;
}

void Character::setName(const std::string& newName) {
    name = newName;
    updateNameTexture();
}

void Character::updateNameTexture() {
    if (nameTexture) {
        SDL_DestroyTexture(nameTexture);
        nameTexture = nullptr;
    }

    if (!nameFont) {
        SDL_Log("Font not loaded, cannot create name texture");
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(nameFont, name.c_str(), nameColor);
    if (!surface) {
        SDL_Log("Failed to create name surface: %s", TTF_GetError());
        return;
    }

    nameTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!nameTexture) {
        SDL_Log("Failed to create name texture: %s", SDL_GetError());
    }
}

void Character::render() {
    // Draw character as a colored rectangle for now
    SDL_Rect destRect = {
        gridX * GRID_SIZE,
        gridY * GRID_SIZE,
        GRID_SIZE,
        GRID_SIZE
    };

    // Render character body
    SDL_SetRenderDrawColor(renderer, clothesColor.r, clothesColor.g, clothesColor.b, clothesColor.a);
    SDL_RenderFillRect(renderer, &destRect);

    // Render name if available
    if (nameTexture) {
        int textWidth, textHeight;
        if (SDL_QueryTexture(nameTexture, nullptr, nullptr, &textWidth, &textHeight) == 0) {
            SDL_Rect nameRect = {
                destRect.x + (GRID_SIZE - textWidth) / 2,
                destRect.y - textHeight - 5,
                textWidth,
                textHeight
            };
            SDL_RenderCopy(renderer, nameTexture, nullptr, &nameRect);
        }
    }
}

void Character::update() {
    // Add any continuous updates here (animations, etc.)
}