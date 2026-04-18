#include "../include/freeze.h"
#include "../include/collectible_manager.h"
#include "../include/game_management.h"

Freeze::Freeze(SDL_Renderer* renderer, int x, int y)
    : Collectible(renderer, "../imgs/freeze.png", x, y, 30000) {  // 30 second respawn
    freezeDuration = 5.0f;  // 5 seconds freeze
    m_active = false;  // Use m_active
    powerupTimer = 0.0f;
    
    SDL_Point spawn = CollectibleManager::getRandomSpawnPoint();
    position.x = spawn.x * GRID_SIZE;
    position.y = spawn.y * GRID_SIZE;

    // Initialize glow parameters
    glowColor = {0, 255, 255, 255};  // Cyan glow
    glowIntensity = 1.2f;
    glowTime = 0.0f;
}

void Freeze::collect() {
    GameManagement::incrementCount("freeze");
    CollectibleManager::playPowerupSound();
    CollectibleManager::createParticles(renderer, position.x, position.y);
    m_active = true;
    powerupTimer = freezeDuration;
    // Update via static method or reference
    CollectibleManager::setFreezeIconActive(true, freezeDuration);
    despawn();
}

void Freeze::update(float deltaTime) {
    Collectible::update(deltaTime);
    if (m_active) {
        powerupTimer -= deltaTime;
        // Update icon with current time remaining
        CollectibleManager::setFreezeIconActive(true, powerupTimer);
        if (powerupTimer <= 0) {
            m_active = false;
            CollectibleManager::setFreezeIconActive(false, 0);
        }
    }
}

void Freeze::renderGlow() {
    if (!m_isVisible) return;
    drawCircularGlow(2.2f, 4, 2.5f, 2.0f);  // Icy effect glow
}

void Freeze::render() {
    if (!m_isVisible) return;
    renderGlow();  // First layer - glow effect
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &position);  // Second layer - texture
    }
    drawProgressRing();  // Final layer - progress ring
}