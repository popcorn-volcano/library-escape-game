#include "../include/vision.h"
#include "../include/constants.h" // Include this for GRID_SIZE
#include <cmath>

Vision::Vision(const Character *owner, int range, float angle)
    : owner(owner), range(range), angle(angle), direction(0) {}

bool Vision::isCharacterInVision(const Character *character) const
{
    int ownerX = owner->getX();
    int ownerY = owner->getY();
    int charX = character->getX();
    int charY = character->getY();

    int dx = charX - ownerX;
    int dy = charY - ownerY;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > range)
    {
        return false;
    }

    float angleToCharacter = std::atan2(dy, dx);

    // Normalize the angle difference to the range [-PI, PI]
    float diff = std::fmod(direction - angleToCharacter + M_PI, 2 * M_PI) - M_PI;
    return std::fabs(diff) <= angle / 2;
}

void Vision::render(SDL_Renderer *renderer) const {
    int ownerX = owner->getX() * GRID_SIZE + GRID_SIZE / 2;
    int ownerY = owner->getY() * GRID_SIZE + GRID_SIZE / 2;

    // Adjust vision height for horizontal movement
    if (direction == 0 || direction == M_PI) { // Moving right or left
        ownerY -= GRID_SIZE / 2.15; // Adjust to higher eye level
    }
    
    // Set blend mode for transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 60);  // Light red with transparency
    
    // Draw filled triangle fan
    const float maxRange = range * GRID_SIZE;
    const float startAngle = direction - angle / 2;
    const float endAngle = direction + angle / 2;
    const float angleStep = 0.02f;  // Small steps for smooth arc
    
    // Create triangle fan from center
    float prevX = ownerX + maxRange * std::cos(startAngle);
    float prevY = ownerY + maxRange * std::sin(startAngle);
    
    for (float currentAngle = startAngle; currentAngle <= endAngle; currentAngle += angleStep) {
        float currentX = ownerX + maxRange * std::cos(currentAngle);
        float currentY = ownerY + maxRange * std::sin(currentAngle);
        
        // Draw filled triangle
        SDL_Vertex vertices[3] = {
            {{static_cast<float>(ownerX), static_cast<float>(ownerY)}, 
             {255, 0, 0, 60}, {0, 0}},
            {{prevX, prevY}, 
             {255, 0, 0, 20}, {0, 0}},
            {{currentX, currentY}, 
             {255, 0, 0, 20}, {0, 0}}
        };
        
        SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
        
        prevX = currentX;
        prevY = currentY;
    }
    
    // Add a slightly brighter outline
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 80);
    float x1 = ownerX + maxRange * std::cos(startAngle);
    float y1 = ownerY + maxRange * std::sin(startAngle);
    float x2 = ownerX + maxRange * std::cos(endAngle);
    float y2 = ownerY + maxRange * std::sin(endAngle);
    
    SDL_RenderDrawLine(renderer, ownerX, ownerY, x1, y1);
    SDL_RenderDrawLine(renderer, ownerX, ownerY, x2, y2);
    
    // Reset blend mode
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Vision::setDirection(float newDirection)
{
    direction = newDirection;
}
