#include "../include/collectible_manager.h"
#include <cstdlib>
#include <ctime>

// Initialize static members
std::vector<SDL_Point> CollectibleManager::spawnPoints;
Mix_Chunk* CollectibleManager::collectSound = nullptr;
Mix_Chunk* CollectibleManager::powerupSound = nullptr;
SDL_Texture* CollectibleManager::particleTexture = nullptr;
bool CollectibleManager::initialized = false;
playground* CollectibleManager::currentPlayground = nullptr;  // Add this line

void CollectibleManager::initialize() {
    if (!initialized) {
        // Initialize spawn points
        spawnPoints = {
            {2, 5}, {3, 5}, {12, 4}, {13, 4},
            {17, 2}, {18, 2}, {22, 4}, {23, 4},
            {27, 5}, {28, 5}, {30, 5}, {31, 5},
            {3, 10}, {4, 10}, {10, 10}, {11, 10},{26, 10}, {27,10}, {2, 16},
            {3, 16}, {7, 16}, {8, 16}, {12, 16}, {13, 16}, {17, 15}, {18, 15}, {22, 16}, {23, 16}, {27,15}, {28, 15}

        };

        // Load sounds
        collectSound = Mix_LoadWAV("../sounds/collect.wav");
        powerupSound = Mix_LoadWAV("../sounds/powerup.wav");
        
        // Initialize random seed
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        
        initialized = true;
    }
}

SDL_Point CollectibleManager::getRandomSpawnPoint() {
    if (!initialized) initialize();
    int index = rand() % spawnPoints.size();
    return spawnPoints[index];
}

void CollectibleManager::playCollectSound() {
    if (collectSound) {
        Mix_PlayChannel(-1, collectSound, 0);
    }
}

void CollectibleManager::playPowerupSound() {
    if (powerupSound) {
        Mix_PlayChannel(-1, powerupSound, 0);
    }
}

void CollectibleManager::createParticles(SDL_Renderer* renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    // Create simple particle effect
    for (int i = 0; i < 8; i++) {
        SDL_Rect particle = {x + 16, y + 16, 4, 4};
        SDL_RenderFillRect(renderer, &particle);
    }
}

void CollectibleManager::cleanup() {
    if (collectSound) {
        Mix_FreeChunk(collectSound);
        collectSound = nullptr;
    }
    if (powerupSound) {
        Mix_FreeChunk(powerupSound);
        powerupSound = nullptr;
    }
}

void CollectibleManager::setCoffeeIconActive(bool active, float duration) {
    if (playground* currentPlayground = getCurrentPlayground()) {
        currentPlayground->setCoffeeIconState(active, duration);
    }
}

void CollectibleManager::setFreezeIconActive(bool active, float duration) {
    if (playground* currentPlayground = getCurrentPlayground()) {
        currentPlayground->setFreezeIconState(active, duration);
    }
}

void CollectibleManager::queueNoteSpawn() {
    if (!currentPlayground) return;
    
    // Get currently used positions
    std::set<SDL_Point, SDLPointCompare> usedPoints;
    for (const auto& collectible : currentPlayground->collectibles) {
        if (collectible->isVisible()) {
            SDL_Rect pos = collectible->getPosition();
            usedPoints.insert({pos.x / GRID_SIZE, pos.y / GRID_SIZE});
        }
    }
    
    // Try to find a free spawn point
    SDL_Point spawnPoint;
    bool foundSpot = false;
    for (int attempts = 0; attempts < 10; attempts++) {  // Limit attempts
        spawnPoint = getRandomSpawnPoint();
        if (usedPoints.count(spawnPoint) == 0) {
            foundSpot = true;
            break;
        }
    }
    
    // Only spawn if we found a free spot
    if (foundSpot) {
        currentPlayground->queueCollectible(
            std::make_unique<Note>(currentPlayground->renderer, 
                                 spawnPoint.x, spawnPoint.y)
        );
    }
}