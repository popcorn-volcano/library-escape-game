#include "../include/collectible.h"
#include "../include/constants.h"

Collectible::Collectible(SDL_Renderer* renderer, const char* texturePath, 
                        int x, int y, int respawnTime) 
    : renderer(renderer), m_isVisible(true), spawnTimer(0), 
      respawnTime(respawnTime) {
    
    texture = loadTexture(texturePath, renderer);
    position = {x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE};  // Default size
}

Collectible::~Collectible() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Collectible::update(float deltaTime) {
    if (!m_isVisible) {
        spawnTimer += deltaTime;
        if (spawnTimer >= respawnTime) {
            spawn();
        }
    }
}

void Collectible::render() {
    if (m_isVisible) {
        renderGlow();  // First layer - glow effect
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &position);  // Second layer - texture
        }
        drawProgressRing();  // Final layer - progress ring
    }
}

void Collectible::renderGlow() {
    // Default glow implementation (can be overridden by derived classes)
    if (m_isVisible) {
        // Create a slightly larger rect for the glow
        SDL_Rect glowRect = {
            position.x - 4,
            position.y - 4,
            position.w + 8,
            position.h + 8
        };
        
        // Set glow blend mode
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        
        // Default white glow
        SDL_Color defaultGlow = {255, 255, 255, 255};
        
        // Calculate alpha based on time
        Uint8 alpha = static_cast<Uint8>(127 + 128 * sin(SDL_GetTicks() * 0.001f));
        
        // Draw glow
        SDL_SetRenderDrawColor(renderer, 
            defaultGlow.r, 
            defaultGlow.g, 
            defaultGlow.b, 
            alpha);
            
        SDL_RenderFillRect(renderer, &glowRect);
        
        // Reset blend mode
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    }
}

void Collectible::spawn() {
    m_isVisible = true;
    spawnTimer = 0;
}

void Collectible::despawn() {
    m_isVisible = false;
}

void Collectible::drawCircularGlow(float frequency, int layerCount, 
                                  float baseRadius, float radiusIncrement) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    
    Uint8 alpha = static_cast<Uint8>(100 + 80 * sin(glowTime * frequency));
    
    int centerX = position.x + position.w / 2;
    int centerY = position.y + position.h / 2;
    
    for (int i = 0; i < layerCount; i++) {
        SDL_SetRenderDrawColor(renderer, 
            glowColor.r, 
            glowColor.g, 
            glowColor.b, 
            alpha / (i + 1.5f));
            
        float radius = position.w/2 + baseRadius + (i * radiusIncrement);
        
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

void Collectible::drawProgressRing() {
    if (!isBeingCollected || !m_isVisible) return;

    // Calculate center and radius with larger offset
    int centerX = position.x + position.w / 2;
    int centerY = position.y + position.h / 2;
    int radius = (position.w / 2) + 12;  // Increased from 6 to 12 for larger ring

    // Set up drawing with higher alpha and brighter color
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 230);  // Increased alpha from 200 to 230

    // Draw thicker progress arc
    float endAngle = collectionProgress * 2 * M_PI;
    for (float angle = 0; angle < endAngle; angle += 0.01f) {
        // Draw multiple points for thicker line
        for (int offset = -1; offset <= 1; offset++) {
            int x = centerX + (radius + offset) * cos(angle - M_PI/2);
            int y = centerY + (radius + offset) * sin(angle - M_PI/2);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

void Collectible::startCollection() {
    isBeingCollected = true;
    collectionProgress = 0.0f;
}

void Collectible::updateCollection(float deltaTime) {
    if (isBeingCollected) {
        collectionProgress += deltaTime / COLLECTION_TIME;
        if (collectionProgress >= 1.0f) {
            collect();
            isBeingCollected = false;
            collectionProgress = 0.0f;
        }
    }
}

void Collectible::cancelCollection() {
    isBeingCollected = false;
    collectionProgress = 0.0f;  // Reset progress to 0
}
