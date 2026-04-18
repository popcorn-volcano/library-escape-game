#include "../include/note.h"
#include "../include/collectible_manager.h"
#include "../include/game_management.h"

Note::Note(SDL_Renderer* renderer, int x, int y)
    : Collectible(renderer, "../imgs/note.png", x, y, 10000) {  // 10 second respawn
    scoreValue = 100;  // Base score value
    // Initialize glow parameters
    glowColor = {255, 255, 255, 255};  // White glow
    glowIntensity = 0.8f;
    glowTime = 0.0f;
}


void Note::update(float deltaTime) {
    Collectible::update(deltaTime);
    glowTime += deltaTime;
    if (glowTime > 2 * M_PI) {
        glowTime -= 2 * M_PI;
    }
}

void Note::renderGlow() {
    if (!m_isVisible) return;
    drawCircularGlow(2.0f, 3, 1.5f, 1.5f);  // Subtle white glow effect
}

void Note::collect() {
    GameManagement::incrementCount("note");
    CollectibleManager::playCollectSound();
    CollectibleManager::createParticles(renderer, position.x, position.y);
    despawn();
    CollectibleManager::queueNoteSpawn();  // This will now use the queuing system
}

void Note::render() {
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