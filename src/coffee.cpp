#include "../include/coffee.h"
#include "../include/collectible_manager.h"
#include "../include/game_management.h"

// Initialize static members

Coffee::Coffee(SDL_Renderer* renderer, int x, int y)
    : Collectible(renderer, "../imgs/coffee.png", x, y, 15000) {
    speedBoostDuration = 15.0f;  // Changed from 20.0f to 15.0f seconds
    speedBoostMultiplier = 1.5f;
    isActive = false;
    powerupTimer = 0.0f;
    
    SDL_Point spawn = CollectibleManager::getRandomSpawnPoint();
    position.x = spawn.x * GRID_SIZE;
    position.y = spawn.y * GRID_SIZE;
    glowColor = {0, 191, 255, 255};  // Deep Sky Blue
    glowIntensity = 1.0f;
    glowTime = 0.0f;  // Add this line
}

void Coffee::collect() {
    GameManagement::incrementCount("coffee");
    GameManagement::setCoffeePowerupActive(true);
    CollectibleManager::playPowerupSound();
    CollectibleManager::createParticles(renderer, position.x, position.y);
    isActive = true;
    float boostDuration = 15.0f;
    powerupTimer = boostDuration;
    CollectibleManager::setCoffeeIconActive(true, boostDuration);
    despawn();
}

void Coffee::update(float deltaTime) {
    Collectible::update(deltaTime);
    glowTime += deltaTime;
    if (glowTime > 2 * M_PI) {
        glowTime -= 2 * M_PI;
    }
    if (isActive) {
        powerupTimer -= deltaTime;
        // Update icon with current time remaining
        CollectibleManager::setCoffeeIconActive(true, powerupTimer);
        if (powerupTimer <= 0) {
            isActive = false;
            GameManagement::setCoffeePowerupActive(false);  // Add this
            CollectibleManager::setCoffeeIconActive(false, 0);
        }
    }
}

void Coffee::renderGlow() {
    if (!m_isVisible) return;
    drawCircularGlow(3.0f, 4, 2.0f, 2.0f);  // Energetic fast-pulsing glow
}

void Coffee::render() {
    if (m_isVisible) {
        // Render glow effect first (behind the collectible)
        renderGlow();
        
        // Then render the collectible texture
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &position);
        }
        drawProgressRing();
    }
}