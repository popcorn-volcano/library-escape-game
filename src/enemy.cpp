#include "../include/enemy.h"
#include "../include/playground.h"
#include "../include/audio_manager.h" // Add audio manager include
#include <cstdlib>
#include <ctime>

Enemy::Enemy(SDL_Renderer *renderer, const std::string &name, const playground *gamePlayground, int startX, int startY, const std::vector<std::pair<int, int>> &waypoints, int speed)
    : Character(renderer, name, gamePlayground, startX, startY), patrolWaypoints(waypoints), currentWaypointIndex(0), patrolSpeed(speed), lastMoveTime(0), vision(this, 5, M_PI / 4), characterDetected(false), detectionTime(0)
{
    std::srand(std::time(nullptr)); // Seed for random direction changes
    loadTextures(renderer); // Load textures for different directions
}

void Enemy::loadTextures(SDL_Renderer *renderer)
{
    textureRight = IMG_LoadTexture(renderer, "../imgs/character_right.png");
    textureLeft = IMG_LoadTexture(renderer, "../imgs/character_left.png");
    textureUp = IMG_LoadTexture(renderer, "../imgs/character_up.png");
    textureDown = IMG_LoadTexture(renderer, "../imgs/character_down.png");

    if (!textureRight || !textureLeft || !textureUp || !textureDown)
    {
        SDL_Log("Failed to load one or more enemy textures: %s", SDL_GetError());
    }
    currentTexture = textureDown; // Default texture
}

void Enemy::update()
{
    // Don't update if the game is paused
    if (getGamePlayground()->isPaused()) // Now calls the getter method
    {
        return;
    }

    Uint32 currentTime = SDL_GetTicks();

    if (characterDetected)
    {
        if (currentTime - detectionTime >= 2000)
        {
            characterDetected = false; // Resume patrol after 2 seconds
        }
        else
        {
            return; // Pause for 2 seconds
        }
    }

    const playground *gamePlayground = getGamePlayground(); // Use the getter method
    if (vision.isCharacterInVision(gamePlayground->getMainCharacter()))
    {
        characterDetected = true;
        detectionTime = currentTime;
        AudioManager::playSoundEffect("detected"); // Use specific ID
        const_cast<playground*>(gamePlayground)->reduceTimer(10.0f);
        return;
    }

    if (currentTime - lastMoveTime < pauseDuration)
    {
        return; // Pause for the specified duration
    }

    int nextX = gridX;
    int nextY = gridY;

    switch (currentWaypointIndex)
    {
    case 0:
        nextX += patrolSpeed;
        vision.setDirection(0); // Move right
        currentTexture = textureRight;
        break;
    case 1:
        nextX -= patrolSpeed;
        vision.setDirection(M_PI); // Move left
        currentTexture = textureLeft;
        break;
    case 2:
        nextY += patrolSpeed;
        vision.setDirection(M_PI / 2); // Move down
        currentTexture = textureDown;
        break;
    case 3:
        nextY -= patrolSpeed;
        vision.setDirection(-M_PI / 2); // Move up
        currentTexture = textureUp;
        break;
    }

    if (nextX < 0 || nextX >= 32 || nextY < 0 || nextY >= 18 || gamePlayground->isPositionBlocked(nextX, nextY) || gamePlayground->isEnemyCollision(nextX, nextY))
    {
        // Change direction randomly
        currentWaypointIndex = std::rand() % 4;
    }
    else
    {
        gridX = nextX;
        gridY = nextY;
        lastMoveTime = currentTime; // Update the last move time
    }
}

void Enemy::render()
{
    SDL_Rect destRect = {
        gridX * GRID_SIZE + (GRID_SIZE - 32) / 2, // Center the sprite horizontally
        gridY * GRID_SIZE + (GRID_SIZE - 64),     // Align the sprite vertically
        32,
        64};

    SDL_RenderCopy(renderer, currentTexture, nullptr, &destRect);
    vision.render(renderer); // Render the vision area
}
