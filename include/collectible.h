#pragma once
#include <SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "tool.h"
#include <cmath>

class Collectible {
protected:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect position;
    bool m_isVisible;
    int spawnTimer;
    int respawnTime;
    bool isBeingCollected = false;
    float collectionProgress = 0.0f;

    void drawProgressRing();

    float value;

    // Base glow parameters
    SDL_Color glowColor;
    float glowIntensity;
    float glowTime;

    // Add these members
    const float COLLECTION_TIME = 0.5f;  // Time to collect in seconds

    // Helper method for derived classes
    void drawCircularGlow(float frequency = 2.5f, 
                         int layerCount = 4, 
                         float baseRadius = 2.0f,
                         float radiusIncrement = 2.0f);

public:
    Collectible(SDL_Renderer* renderer, const char* texturePath, int x, int y, int respawnTime);
    virtual ~Collectible();
    
    virtual void update(float deltaTime);
    virtual void render();
    virtual void collect() = 0;
    virtual void spawn();
    virtual void despawn();

    // Add glow methods
    virtual void renderGlow();  // Virtual method for glow effect

    // Add these methods
    void startCollection();
    void updateCollection(float deltaTime);
    void cancelCollection();

    // Add this getter method
    bool getIsBeingCollected() const { return isBeingCollected; }

    bool isVisible() const { return m_isVisible; }  // Access renamed member variable
    SDL_Rect getPosition() const { return position; }
};