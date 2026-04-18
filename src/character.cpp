#include "../include/character.h"
#include "../include/tool.h"
#include "../include/playground.h" // Add this include

Character::Character(SDL_Renderer *renderer, const std::string &name,
                     const playground *playground, int startX, int startY)
    : renderer(renderer), name(name), gamePlayground(playground),
      gridX(startX), gridY(startY), facingRight(true),
      sprite(nullptr), nameFont(nullptr), nameTexture(nullptr), // Initialize all pointers
      skinColor{255, 200, 150, 255},                            // Default skin color
      clothesColor{50, 50, 200, 255},                           // Default clothes color
      hairColor{100, 50, 0, 255},                               // Default hair color
      nameColor{255, 255, 255, 255},                            // Default name color
      haloColor{255, 239, 120, 255},                              // Gold color for halo
      haloRadius(40.0f),
      haloAlpha(30),
      haloAnimationTime(0),
      showHalo(true) // Enable halo by default
{
    SDL_Log("Initializing character...");

    // Initialize font for name tag
    nameFont = TTF_OpenFont("../fonts/Action_Man_Bold.ttf", 16);
    if (!nameFont)
    {
        SDL_Log("Failed to load name font: %s", TTF_GetError());
        return; // Early return on failure
    }
    SDL_Log("Name font loaded.");

    // Create name texture
    updateNameTexture();
    if (!nameTexture)
    {
        SDL_Log("Failed to create name texture!");
        return;
    }
    SDL_Log("Name texture created.");

    loadTextures(renderer); // Load textures for different directions

    SDL_Log("Character initialized successfully.");
}

void Character::loadTextures(SDL_Renderer *renderer)
{
    textureRight = IMG_LoadTexture(renderer, "../imgs/character_right.png");
    textureLeft = IMG_LoadTexture(renderer, "../imgs/character_left.png");
    textureUp = IMG_LoadTexture(renderer, "../imgs/character_up.png");
    textureDown = IMG_LoadTexture(renderer, "../imgs/character_down.png");

    if (!textureRight || !textureLeft || !textureUp || !textureDown)
    {
        SDL_Log("Failed to load one or more character textures: %s", SDL_GetError());
    }
    currentTexture = textureDown; // Default texture
}

Character::~Character()
{
    if (sprite)
    {
        SDL_DestroyTexture(sprite);
    }
    if (nameTexture)
    {
        SDL_DestroyTexture(nameTexture);
    }
    if (nameFont)
    {
        TTF_CloseFont(nameFont);
    }
    if (textureRight)
    {
        SDL_DestroyTexture(textureRight);
    }
    if (textureLeft)
    {
        SDL_DestroyTexture(textureLeft);
    }
    if (textureUp)
    {
        SDL_DestroyTexture(textureUp);
    }
    if (textureDown)
    {
        SDL_DestroyTexture(textureDown);
    }
}

void Character::moveLeft()
{
    if (gridX > 0 && !gamePlayground->isPositionBlocked(gridX - 1, gridY))
    {
        gridX--;
        facingRight = false;
        currentTexture = textureLeft;
    }
}

void Character::moveRight()
{
    if (gridX < 31 && !gamePlayground->isPositionBlocked(gridX + 1, gridY))
    {
        gridX++;
        facingRight = true;
        currentTexture = textureRight;
    }
}

void Character::moveUp()
{
    if (gridY > 0 && !gamePlayground->isPositionBlocked(gridX, gridY - 1))
    {
        gridY--;
        currentTexture = textureUp;
    }
}

void Character::moveDown()
{
    if (gridY < 17 && !gamePlayground->isPositionBlocked(gridX, gridY + 1))
    {
        gridY++;
        currentTexture = textureDown;
    }
}

void Character::setSkinColor(Uint8 r, Uint8 g, Uint8 b)
{
    skinColor = {r, g, b, 255};
}

void Character::setClothesColor(Uint8 r, Uint8 g, Uint8 b)
{
    clothesColor = {r, g, b, 255};
}

void Character::setHairColor(Uint8 r, Uint8 g, Uint8 b)
{
    hairColor = {r, g, b, 255};
}

void Character::setHairStyle(int style)
{
    hairStyle = style;
}

void Character::setName(const std::string &newName)
{
    name = newName;
    updateNameTexture();
}

void Character::updateNameTexture()
{
    if (nameTexture)
    {
        SDL_DestroyTexture(nameTexture);
        nameTexture = nullptr;
    }

    if (!nameFont)
    {
        SDL_Log("Font not loaded, cannot create name texture");
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(nameFont, name.c_str(), nameColor);
    if (!surface)
    {
        SDL_Log("Failed to create name surface: %s", TTF_GetError());
        return;
    }

    nameTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!nameTexture)
    {
        SDL_Log("Failed to create name texture: %s", SDL_GetError());
    }
}

void Character::render()
{
    if (showHalo)
    {
        // Calculate halo position (centered on character)
        int centerX = gridX * GRID_SIZE + GRID_SIZE / 2;
        int centerY = gridY * GRID_SIZE + GRID_SIZE / 2;
        
        // Animate halo alpha
        haloAnimationTime += 0.016f;
        float pulseAlpha = (sin(haloAnimationTime * 3.0f) + 1.0f) * 0.5f;
        Uint8 currentAlpha = static_cast<Uint8>(haloAlpha * pulseAlpha);
        
        // Set blend mode for transparency
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        
        // Draw filled circle using SDL2_gfx's filledCircleRGBA
        for (float r = haloRadius; r > 0; r -= 1.0f)
        {
            // Calculate alpha based on radius (outer edge is more transparent)
            float alphaMultiplier = r / haloRadius;
            Uint8 alpha = static_cast<Uint8>(currentAlpha * alphaMultiplier);
            
            for (int dy = -r; dy <= r; dy++)
            {
                for (int dx = -r; dx <= r; dx++)
                {
                    if (dx*dx + dy*dy <= r*r)  // Check if point is inside circle
                    {
                        SDL_SetRenderDrawColor(renderer, 
                            haloColor.r, 
                            haloColor.g, 
                            haloColor.b, 
                            alpha);
                        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
                    }
                }
            }
        }
        
        // Reset blend mode
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }

    SDL_Rect destRect = {
        gridX * GRID_SIZE + (GRID_SIZE - 32) / 2, // Center the sprite horizontally
        gridY * GRID_SIZE + (GRID_SIZE - 64),     // Align the sprite vertically
        32,
        64};

    // Render character texture
    SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);

    // Render name if available
    if (nameTexture)
    {
        int textWidth, textHeight;
        if (SDL_QueryTexture(nameTexture, nullptr, nullptr, &textWidth, &textHeight) == 0)
        {
            SDL_Rect nameRect = {
                destRect.x + (32 - textWidth) / 2,
                destRect.y - textHeight - 5,
                textWidth,
                textHeight};
            SDL_RenderCopy(renderer, nameTexture, nullptr, &nameRect);
        }
    }
}

void Character::update()
{
    // Add any continuous updates here (animations, etc.)
}