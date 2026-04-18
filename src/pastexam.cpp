#include "../include/pastexam.h"
#include "../include/collectible_manager.h"
#include "../include/game_management.h"

PastExam::PastExam(SDL_Renderer* renderer, int x, int y)
    : Collectible(renderer, "../imgs/pastexam.png", x, y, 20000) {
    scoreMultiplier = 2;
    duration = 15.0f; // 15 seconds
    
    // Initialize glow parameters
    glowColor = {255, 215, 0, 255};  // Golden glow for past exam
    glowIntensity = 1.0f;
    glowTime = 0.0f;
    
    SDL_Point spawn = CollectibleManager::getRandomSpawnPoint();
    position.x = spawn.x * GRID_SIZE;
    position.y = spawn.y * GRID_SIZE;
}

void PastExam::collect() {
    GameManagement::incrementCount("exam");
    CollectibleManager::playCollectSound();
    CollectibleManager::createParticles(renderer, position.x, position.y);
    despawn();
    
    // Spawn a new exam through the playground
    if (CollectibleManager::getCurrentPlayground()) {
        CollectibleManager::getCurrentPlayground()->spawnNewExam();
    }
}

void PastExam::render() {
    if (m_isVisible) {
        // Render glow effect first (behind the collectible)
        renderGlow();
        
        // Render the collectible texture
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &position);
        }
        drawProgressRing();
    }
}

void PastExam::renderGlow() {
    if (!m_isVisible) return;
    
    // Set glow blend mode
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    
    // Smoother alpha calculation with adjusted frequency and range
    Uint8 alpha = static_cast<Uint8>(100 + 80 * sin(glowTime * 2.5f));
    
    // Calculate center of the collectible
    int centerX = position.x + position.w / 2;
    int centerY = position.y + position.h / 2;
    
    // Draw multiple circles with decreasing alpha for glow effect
    for (int i = 0; i < 4; i++) {  // Increased layers for smoother transition
        SDL_SetRenderDrawColor(renderer, 
            glowColor.r, 
            glowColor.g, 
            glowColor.b, 
            alpha / (i + 1.5f));  // Smoother alpha falloff
            
        // Smaller base radius and gentler increase per layer
        int radius = position.w/2 + 2 + (i * 2);  // Reduced from 4+(i*4) to 2+(i*2)
        
        // Draw circle
        for (int w = -radius; w <= radius; w++) {
            for (int h = -radius; h <= radius; h++) {
                if (w*w + h*h <= radius*radius) {
                    SDL_RenderDrawPoint(renderer, centerX + w, centerY + h);
                }
            }
        }
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void PastExam::update(float deltaTime) {
    Collectible::update(deltaTime);
    glowTime += deltaTime;
    if (glowTime > 2 * M_PI) {
        glowTime -= 2 * M_PI;
    }
}